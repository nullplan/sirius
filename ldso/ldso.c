#define __STARTUP_CODE
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <elf.h>
#include <pthread.h>
#include <setjmp.h>
#include "cpu.h"
#include "libc.h"
#include "reloc_arch.h"

#ifndef __SIXTY_FOUR
typedef Elf32_Sym Sym;
typedef Elf32_Ehdr Ehdr;
typedef Elf32_Phdr Phdr;
#define R_SYM(i) ELF32_R_SYM(i)
#define R_TYPE(i) ELF32_R_TYPE(i)
#else
typedef Elf64_Sym Sym;
typedef Elf64_Ehdr Ehdr;
typedef Elf64_Phdr Phdr;
#define R_SYM(i) ELF64_R_SYM(i)  ((i)>>32)
#define R_TYPE(i) ELF64_R_TYPE(i) ((i)&0xffffffffU)
#endif
struct ldso {
    uintptr_t base;
    Ehdr *eh;
    const size_t *dynv;
    const char *name;
    const char *shortname;
    const uint32_t *hashtab;
    const uint32_t *ghashtab; /* XXX: check type */
    const Sym *symtab;
    const char *strtab;
    int relocated;
    struct ldso *next;
};

static struct ldso main, libc;
static struct ldso *head;
static const size_t *libc_original_addends;
static int ldd_mode;
static int early_error;
static int runtime;

static void process_dynv(struct ldso *dso)
{
    for (size_t *d = dso->dynv; *d; d += 2)
    {
        uintptr_t p = dso->base + d[1];
        switch (*d) {
            case DT_HASH:       dso->hashtab = (void *)p; break;
            case DT_GNU_HASH:   dso->ghashtab = (void *)p; break;
            case DT_SYMTAB:     dso->symtab = (void *)p; break;
            case DT_STRTAB:     dso->strtab = (void *)p; break;
        }
    }
}

static size_t scan_vec(const size_t *vec, size_t tag)
{
    for (; *vec; vec += 2)
        if (*vec == tag)
            return vec[1];
    return 0;
}

static void decode_vec(size_t *arr, const size_t *vec, size_t lim)
{
    for (; *vec; vec += 2)
    {
        if (*vec < 8 * sizeof (size_t))
            arr[0] |= 1ul << *vec;
        if (*vec < lim)
            arr[vec[0]] = vec[1];
    }
}

struct symdef {
    struct ldso *dso;
    Sym *sym;
};

#define OK_BINDS        (1ul << STB_GLOBAL | 1ul << STB_WEAK)
#define OK_TYPES        (1ul << STT_NOTYPE  | 1ul << STT_OBJECT | 1ul << STT_FUNC | 1ul << STT_COMMON | 1ul << STT_TLS)
static struct symdef find_sym(const char *name, struct ldso *ctx)
{
    uint32_t gh = gnu_hash(name);
    uint32_t h = sysv_hash(name);
    size_t i;
    for (; ctx; ctx = ctx->next)
    {
        i = 0;
        if (ctx->ghashtab)
            i = lookup_gnu(ctx->ghashtab, name, gh);
        else if (ctx->hashtab)
            i = lookup_sysv(ctx->hashtab, name, h);
        if (i)
        {
            Sym *sym = ctx->symtab + i;
            if (((1ul << (sym->st_info >> 4)) & OK_BINDS)
                    && ((1ul << (sym->st_info & 0xf)) & OK_TYPES))
                return (struct symdef){ctx, sym};
    }
    return (struct symdef){0};
}

static void process_relocs(struct ldso *dso, const size_t *rel, size_t relsz, size_t stride)
{
    int skip_rels = dso == &libc;
    int reuse_addends = dso == &libc && stride == 2;

    for (; relsz; relsz -= stride * sizeof (size_t), rel += stride)
    {
        int type = R_TYPE(rel[1]);
        if (skip_rels && type == REL_RELATIVE)
            continue;

        size_t *rel_addr = (void *)(dso->base + rel[0]);
        size_t addend = stride == 3? rel[2] :
            reuse_addends? *libc_original_addends++ :
            *rel_addr;
        Sym *usym = dso->symtab + R_SYM(rel[1]);
        struct symdef def = find_sym(dso->strtab + usym->st_name, type == REL_COPY? dso->next : head);
        size_t symval = def.sym? def.dso->base + def.sym->st_value : 0;
        if (!def.sym && (usym->st_info >> 4) != STB_WEAK)
            print_error("error relocating `%s': symbol `%s' not found", dso->shortname, dso->strtab + usym->st_name);
        else
        {
            switch (type) {
                case REL_RELATIVE:
                    *rel_addr = dso->base + addend;
                    break;

                case REL_COPY:
                    if (usym->st_size != def.sym->st_size)
                        print_error("error relocating `%s': Copy rel size mismatch (expected %zu, got %zu)", dso->shortname, usym->st_size, def.sym->st_size);
                    else
                        memcpy(rel_addr, (void *)symval, usym->st_size);
                    break;

                default:
                    print_error("error relocating `%s': Unknown relocation type %d", dso->shortname, type);
                    break;
            }
        }
    }
}

static void process_relr(size_t base, const size_t *rel, size_t sz)
{
    size_t *rel_addr = 0;
    for (; sz; sz -= 8, rel++)
    {
        size_t w = *rel;
        if (!(w & 1)) {
            rel_addr = (void *)(base + w);
            *rel_addr++ += base;
        } else {
            w >>= 1;
            for (int cnt = 8 * sizeof (size_t) - 1; cnt; cnt--, w >>= 1) {
                if (w & 1)
                    *rel_addr += base;
                rel_addr++;
            }
        }
    }
}

static void relocate(struct ldso *dso)
{
    if (dso->relocated) return;
    size_t dyn[DT_RELR + 1] = {0};
    decode_vec(dyn, dso->dynv, DT_RELR + 1);
    process_relocs(dso, (void *)(dso->base + dyn[DT_REL]), dyn[DT_RELSZ], 2);
    process_relocs(dso, (void *)(dso->base + dyn[DT_RELA]), dyn[DT_RELASZ], 3);
    process_relocs(dso, (void *)(dso->base + dyn[DT_JMPREL]), dyn[DT_PLTRELSZ], dyn[DT_PLTREL] == DT_REL? 2 : 3);
    /* relr table is all relative, so we skipt it for libc, since it has already been processed. */
    if (dso != &libc)
        process_relr(dso->base, (void *)(dso->base + dyn[DT_RELR]), dyn[DT_RELRSZ]);
}

static _Noreturn void setup_tmp_threadptr(long *sp, const size_t *dynv);
static _Noreturn void load_run_remaining(long *sp, const size_t *dynv);

_Noreturn void _start_c(long *sp, const size_t *dynv, long base)
{
    /* first stage (rcrt1s.S) already processed relative relocs.
     * Therefore we can already use local functions and data.
     * We need to initialize the dynlinker enough to be able to process our symbolic relocs.
     * Then do so and go to the next stage.
     */
    head = &libc;
    libc.base = base;
    libc.dynv = dynv;
    libc.name = libc.shortname = "libc.so";

    process_dynv(&libc);

    /* we need to perform symbolic relocation now,
     * but we need to re-perform it after loading all other libs.
     * This works with DT_RELA, but not with DT_REL,
     * so we need to save the original addends first.
     *
     * To that end we count the symbolic DT_REL relocations.
     */
    size_t reloff = scan_vec(dynv, DT_REL);
    size_t relsz = scan_vec(dynv, DT_RELSZ);
    size_t relsymcnt = 0;
    const size_t *const rel = (void *)(base + reloff);
    for (size_t i = 0, left = relsz; left; i += 2, left -= 2 * sizeof (size_t))
    {
        if (R_TYPE(rel[i+1]) != REL_RELATIVE)
            relsymcnt++;
    }

    /* there shouldn't be too many like that, so if we do have a large number of them,
     * just crash.
     */
    if (relsymcnt > 4096) a_crash();

    size_t original_addends[relsymcnt? relsymcnt : 1];
    for (size_t i = 0, left = relsz, j = 0; left; i++, left -= 2 * sizeof (size_t))
    {
        if (R_TYPE(rel[2*i+1]) != REL_RELATIVE)
            original_addends[j++] = *(size_t *)(base + rel[2*i]);
    }

    libc_original_addends = original_addends;
    relocate(&libc);
    libc.relocated = 0;

    void (*next_stage)(long *sp, const size_t *dynv) = setup_tmp_threadptr;
    __asm__ volatile("" : "+r"(next_stage) :: "memory");
    next_stage(sp, dynv);
    for (;;);
}

static _Noreturn void setup_tmp_threadptr(long *sp, const size_t *dynv)
{
    /* we set up a temporary thread pointer to use for holding errno, hwcap, and sysinfo */
    /* also, cause it's on the way, we set __environ for use with getenv() later. */
    size_t aux[AT_SYSINFO + 1];
    struct __pthread tp = {0};
    tp.self = tp.next = tp.prev = &tp;
    size_t *auxv = (void *)(sp + 2 + *sp);
    __environ = (void *)auxv;
    while (*auxv++);
    decode_vec(aux, auxv, AT_SYSINFO + 1);
    if (aux[AT_SYSINFO]) __sysinfo = aux[AT_SYSINFO];
    tp.sysinfo = aux[AT_SYSINFO];
    tp.hwcap = aux[AT_HWCAP];
    if (__set_thread_area(__tp_adjust(&tp)))
        a_crash();

    void (*next_stage)(long *sp, const size_t *dynv) = load_run_remaining;
    __asm__ volatile("" : "+r"(next_stage) :: "memory");
    next_stage(sp, dynv);
    for (;;);
}

#define AUX_CNT (AT_EXECFN + 1)
static _Noreturn void load_run_remaining(long *sp, const size_t *dynv)
{
    int argc = *sp;
    char **argv = (void *)(sp + 1);
    char **envp = argv + argc + 1;
    __environ = envp;

    char *lib_path = 0;
    char *preload = 0;
    const size_t *auxv = (void *)envp;
    struct ldso *app = &main;
    while (*auxv++);

    size_t aux[AUX_CNT] = {0};
    decode_vec(aux, auxv, AUX_CNT);

    __page_size = aux[AT_PAGESZ];

    int restricted = (aux[0] & 0x7800) != 0x7800 || aux[AT_UID] != aux[AT_EUID] || aux[AT_GID] != aux[AT_EGID] || aux[AT_SECURE];
    if (!restricted)
    {
        lib_path = getenv("LD_LIBRARY_PATH");
        preload = getenv("LD_PRELOAD");
    }
    if (!lib_path) lib_path = "/lib:/usr/lib";
    if (aux[AT_BASE]) {
        /* we were called as dynamic linker */
        main.name = (void *)aux[AT_EXECFN];
        main.shortname = argv[0];
        main.eh = (void *)(aux[AT_PHDR] & -PAGE_SIZE);
        main.dyn = find_dynamic(main.eh, (void *)aux[AT_PHDR], aux[AT_PHNUM], aux[AT_PHENT]);
        if (main.dyn) process_dynv(&main);
    } else {
        /* we were called as command */
        int optoff;
        for (optoff = 1; optoff < argc; optoff++)
        {
            if (!strcmp(argv[optoff], "--list"))
                ldd_mode = 1;
            else if (!strcmp(argv[optoff], "--library-path") && optoff < argc - 1)
                lib_path = argv[++optoff];
            else if (!strcmp(argv[optoff], "--preload") && optoff < argc - 1)
                preload = argv[++optoff];
            else break;
        }
        if (optoff == argc) {
            print_error("sirius loader (v0.0.1)");
            _Exit(0);
        }
        argc -= optoff;
        argv += optoff;
        app = load_library(argv[0]);
        if (app)
            aux[AT_ENTRY] = app->base + app->eh->e_entry;
    }

    app->next = &libc;
    libc.next = 0;

    if (preload) {
        struct ldso *pl = load_library(preload);
        if (pl) {
            head = pl;
            pl->next = app;
        }
    }

    load_deps(head);
    reloc_all(head);
    if (early_error)
        _Exit(1);
    if (ldd_mode) {
        print_libs();
        _Exit(0);
    }

    runtime = 1;
    a_stackjmp(aux[AT_ENTRY], sp);
    for (;;);
}

