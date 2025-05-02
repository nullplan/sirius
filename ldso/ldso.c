#define __STARTUP_CODE
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <elf.h>
#include <pthread.h>
#include <setjmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>
#include "cpu.h"
#include "libc.h"
#include "reloc_arch.h"

#ifndef __SIXTY_FOUR
typedef Elf32_Sym Sym;
typedef Elf32_Ehdr Ehdr;
typedef Elf32_Phdr Phdr;
#define ELFCLASS ELFCLASS32
#define R_SYM(i) ELF32_R_SYM(i)
#define R_TYPE(i) ELF32_R_TYPE(i)
#define ST_TYPE ELF32_ST_TYPE
#define ST_BIND ELF32_ST_BIND
#else
typedef Elf64_Sym Sym;
typedef Elf64_Ehdr Ehdr;
typedef Elf64_Phdr Phdr;
#define ELFCLASS ELFCLASS64
#define R_SYM(i) ELF64_R_SYM(i)
#define R_TYPE(i) ELF64_R_TYPE(i)
#define ST_TYPE ELF64_ST_TYPE
#define ST_BIND ELF64_ST_BIND
#endif
struct ldso {
    uintptr_t base;
    void *map;
    size_t map_len;
    const size_t *dynv;
    dev_t dev;
    ino_t ino;
    const char *name;
    const char *shortname;
    const uint32_t *hashtab;
    const uint32_t *ghashtab; /* XXX: check type */
    const Sym *symtab;
    const char *strtab;
    int relocated;
    int kernel_mapped;
    struct ldso *const *deps;
    struct ldso *next, *prev;
};

static struct ldso main, libc;
static struct ldso *head, *tail;
static const size_t *libc_original_addends;
static int ldd_mode;
static int early_error;
static int runtime;
static struct ldso *const no_deps[] = {0};
static struct ldso *builtin_deps[2];

static void print_error(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    if (!runtime) {
        vdprintf(2, fmt, ap);
        write(2, "\n", 1);
        early_error = 1;
    }
    va_end(ap);
}

static void process_dynv(struct ldso *dso)
{
    for (const size_t *d = dso->dynv; *d; d += 2)
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

static uint32_t gnu_hash(const char *name)
{
    const unsigned char *u = (void *)name;
    uint_fast32_t h = 5381;
    while (*u)
        h = 33 * h + *u++;
    return h;
}

static uint32_t sysv_hash(const char *name)
{
    const unsigned char *u = (void *)name;
    uint_fast32_t h = 0;
    while (*u) {
        h = (h << 4) + *u++;
        h ^= (h >> 24) & 0xf0;
    }
    return h & 0x0fffffff;
}

static size_t lookup_gnu(const struct ldso *dso, const char *name, uint32_t gh)
{
    /* have I mentioned the GNU hash table is a bit more difficult? */
    size_t nbuckets = dso->ghashtab[0];
    size_t symoffset = dso->ghashtab[1];
    size_t bloom_size = dso->ghashtab[2];
    size_t bloom_shift = dso->ghashtab[3];
    const size_t *bloom = (void *)(dso->ghashtab + 4);
    const uint32_t *buckets = (void *)(bloom + bloom_size);
    const uint32_t *chain = buckets + nbuckets;

    size_t bloomword = bloom[(gh / (8 * sizeof (size_t))) % bloom_size];
    size_t test = (1ul << (gh % (8 * sizeof (size_t)))) | (1ul << ((gh >> bloom_shift) % (8 * sizeof (size_t))));
    if ((bloomword & test) != test) return 0;

    gh |= 1;
    size_t idx = buckets[gh % nbuckets] - 1;
    do {
        idx++;
        if ((chain[idx - symoffset] | 1) == gh) {
            const Sym *sym = dso->symtab + idx;
            if (!strcmp(dso->strtab + sym->st_name, name))
                return idx;
        }
    } while (!(chain[idx - symoffset] & 1));
    return 0;
}

static size_t lookup_sysv(const struct ldso *dso, const char *name, uint32_t h)
{
    const uint32_t *bucket = dso->hashtab + 2;
    const uint32_t *chain = bucket + dso->hashtab[0];
    for (size_t idx = bucket[h % dso->hashtab[0]]; idx; idx = chain[idx])
    {
        const Sym *sym = dso->symtab + idx;
        if (!strcmp(dso->strtab + sym->st_name, name))
            return idx;
    }
    return 0;
}

struct symdef {
    struct ldso *dso;
    const Sym *sym;
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
            i = lookup_gnu(ctx, name, gh);
        else if (ctx->hashtab)
            i = lookup_sysv(ctx, name, h);
        if (i)
        {
            const Sym *sym = ctx->symtab + i;
            if (((1ul << (sym->st_info >> 4)) & OK_BINDS)
                    && ((1ul << (sym->st_info & 0xf)) & OK_TYPES))
                return (struct symdef){ctx, sym};
        }
    }
    return (struct symdef){0};
}

static void process_relocs(struct ldso *dso, const size_t *rel, size_t relsz, size_t stride)
{
    int skip_rels = dso == &libc;
    int reuse_addends = dso == &libc && stride == 2;
    size_t addend_idx = 0;

    for (; relsz; relsz -= stride * sizeof (size_t), rel += stride)
    {
        int type = R_TYPE(rel[1]);
        if (skip_rels && type == REL_RELATIVE)
            continue;

        size_t *rel_addr = (void *)(dso->base + rel[0]);
        size_t addend = stride == 3? rel[2] :
            reuse_addends? libc_original_addends[addend_idx++] :
            *rel_addr;

        size_t symval = 0;
        const Sym *usym = 0;
        struct symdef def = {0};
        if (R_SYM(rel[1])) {
            usym = dso->symtab + R_SYM(rel[1]);
            def = usym->st_info >> 4 == STB_LOCAL? (struct symdef){dso, usym} : find_sym(dso->strtab + usym->st_name, type == REL_COPY? dso->next : head);
            symval = def.sym? def.dso->base + def.sym->st_value : 0;
            if (!def.sym && (usym->st_info >> 4) != STB_WEAK)
                print_error("error relocating `%s': symbol `%s' not found", dso->shortname, dso->strtab + usym->st_name);
        }
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
    /* relr table is all relative, so we skip it for libc, since it has already been processed. */
    if (dso != &libc)
        process_relr(dso->base, (void *)(dso->base + dyn[DT_RELR]), dyn[DT_RELRSZ]);
}

static void reloc_all(struct ldso *dso)
{
    for (; dso; dso = dso->next)
        relocate(dso);
}

static int prot_from_flags(int flg)
{
    int rv = 0;
    if (flg & PF_R) rv |= PROT_READ;
    if (flg & PF_W) rv |= PROT_WRITE;
    if (flg & PF_X) rv |= PROT_EXEC;
    return rv;
}

static void *map_library(int fd, struct ldso *dso)
{
    union {
        Ehdr eh;
        char b[1024];
    } buf;
    ssize_t rd = read(fd, buf.b, sizeof buf);
    if (rd < 0) {
        print_error("`%s': Read error: %m", dso->name);
        return 0;
    }

    if ((size_t)rd < sizeof (Ehdr)) {
        print_error("`%s': File too short", dso->name);
        return 0;
    }

    if (memcmp(buf.b, "\177ELF", 4)
            || buf.eh.e_ident[EI_CLASS] != ELFCLASS
            || buf.eh.e_machine != ELFMACH
            || (buf.eh.e_type != ET_DYN && buf.eh.e_type != ET_EXEC)) {
        print_error("`%s': Bad magic", dso->name);
        return 0;
    }

    Phdr *ph0, *ph;
    int elftype = buf.eh.e_type;
    size_t phent = buf.eh.e_phentsize;
    size_t phnum = buf.eh.e_phnum;
    if (buf.eh.e_phoff + phnum * phent <= rd)
        ph0 = (void *)(buf.b + buf.eh.e_phoff);
    else {
        rd = pread(fd, buf.b, phent * phnum, buf.eh.e_phoff);
        if (rd != phent * phnum) {
            print_error("`%s: Failed to read program headers", dso->name);
            return 0;
        }
        ph0 = (void *)buf.b;
    }

    size_t min_address = SIZE_MAX;
    size_t max_address = 0;
    size_t min_offset = 0;
    int minflags = 0;
    ph = ph0;
    for (size_t i = 0; i < phnum; i++, ph = (void *)((char *)ph + phent)) {
        if (ph->p_type == PT_LOAD) {
            if (ph->p_vaddr < min_address) {
                min_address = ph->p_vaddr;
                minflags = ph->p_flags;
                min_offset = ph->p_offset;
            }
            if (ph->p_vaddr + ph->p_memsz > max_address)
                max_address = ph->p_vaddr + ph->p_memsz;
        }
    }

    min_address &= -PAGE_SIZE;
    min_offset &= -PAGE_SIZE;
    max_address = (max_address + PAGE_SIZE - 1) & -PAGE_SIZE;

    void *map = mmap((void *)min_address, max_address - min_address, prot_from_flags(minflags), MAP_PRIVATE, fd, min_offset);
    if (map == MAP_FAILED) {
        print_error("Error loading `%s': %m", dso->name);
        return 0;
    }

    if (elftype == ET_EXEC && map != (void *)min_address) {
        print_error("Error loading `%s': Non-relocatable object could not be loaded to preferred address", dso->name);
        goto out_unmap;
    }
    dso->base = (uintptr_t)map - min_address;

    ph = ph0;
    for (size_t i = 0; i < phnum; i++, ph = (void *)((char *)ph + phent)) {
        if (ph->p_type == PT_DYNAMIC) {
            dso->dynv = (void *)(dso->base + ph->p_vaddr);
        }
        if (ph->p_type == PT_LOAD) {
            size_t this_min = ph->p_vaddr & -PAGE_SIZE;
            size_t this_off = ph->p_offset & -PAGE_SIZE;
            size_t this_max = (ph->p_vaddr + ph->p_memsz + PAGE_SIZE - 1) & -PAGE_SIZE;
            /* reuse first segment */
            if (this_min != min_address
                    && mmap((void *)(dso->base + this_min), this_max - this_min, prot_from_flags(ph->p_flags), MAP_PRIVATE | MAP_FIXED, fd, this_off) == MAP_FAILED) {
                print_error("Error loading `%s': %m", dso->name);
                goto out_unmap;
            }
            if (ph->p_memsz > ph->p_filesz && (ph->p_flags & PF_W)) {
                char *eoi = (char *)(dso->base + ph->p_vaddr + ph->p_filesz);
                char *pagebrk = (char *)(((uintptr_t)eoi + PAGE_SIZE - 1) & -PAGE_SIZE);
                memset(eoi, 0, pagebrk - eoi);
                if (ph->p_memsz - ph->p_filesz > pagebrk - eoi
                        && mmap(pagebrk, ph->p_memsz - (pagebrk - eoi), prot_from_flags(ph->p_flags), MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0) == MAP_FAILED) {
                    print_error("Error loading `%s': %m", dso->name);
                    goto out_unmap;
                }
            }
        }
    }
    process_dynv(dso);
    if (!dso->symtab || !dso->strtab || (!dso->hashtab && !dso->ghashtab))
    {
        print_error("Error loading `%s': Library lacks vital data.", dso->name);
        goto out_unmap;
    }
    dso->map = map;
    dso->map_len = max_address - min_address;
    return map;

out_unmap:
    munmap(map, max_address - min_address);
    return 0;
}

static const char *const libc_alias[] = {
    "c", "pthread", "m", "rt", "xnet", "dl", "util"
};

static struct ldso *load_libc(int variant)
{
    if (ldd_mode) {
        static unsigned reported;
        if (!(reported & (1u << variant))) {
            reported |= 1u << variant;
            dprintf(1, "\tlib%s.so => %s (0x%0*tx)\n", libc_alias[variant], libc.name, 2 * sizeof (size_t), libc.base);
        }
    }

    if (!libc.prev) {
        tail->next = &libc;
        libc.prev = tail;
        tail = &libc;
    }
    return &libc;
}

static int path_open(const char *name, const char *search_path, char *namebuf, size_t namesize)
{
    const char *a, *z;
    size_t namelen = strlen(name);
    if (*search_path) {
        a = search_path;
        for (;;) {
            z = a + __stridx(a, ':');
            if (z - a + 2 + namelen < namesize) {
                if (a == z)
                    memcpy(namebuf, name, namelen + 1);
                else {
                    memcpy(namebuf, a, z - a);
                    namebuf[z-a] = '/';
                    memcpy(namebuf + (z-a) + 1, name, namelen + 1);
                }
                int fd = open(namebuf, O_RDONLY | O_CLOEXEC);
                if (fd != -1) return fd;
            }
            if (!*z) break;
            a = z + 1;
        }
    }
    errno = ENOENT;
    return -1;
}

static struct ldso *load_library(const char *name, const char *search_path)
{
    if (!*name) {
        errno = EINVAL;
        return 0;
    }

    if (!strncmp(name, "lib", 3)) {
        for (size_t i = 0; i < sizeof libc_alias / sizeof *libc_alias; i++) {
            size_t l = strlen(libc_alias[i]);
            if (!strncmp(name + 3, libc_alias[i], l) && name[3+l] == '.') {
                return load_libc(i);
            }
        }
    }

    const char *fullname;
    char namebuf[2 * NAME_MAX + 2];
    int fd;
    if (strchr(name, '/')) {
        fullname = name;
        fd = open(name, O_RDONLY | O_CLOEXEC);
    } else {
        if (strnlen(name, NAME_MAX + 1) > NAME_MAX)
            return 0;

        for (struct ldso *dso = head; dso; dso = dso->next)
            if (dso->shortname && !strcmp(dso->shortname, name))
                return dso;

        fullname = namebuf;
        fd = path_open(name, search_path, namebuf, sizeof namebuf);
    }

    if (fd == -1) {
        print_error("Error loading `%s': %m", name);
        return 0;
    }
    struct stat st;
    if (fstat(fd, &st)) {
        print_error("Error stating `%s': %m", name);
        close(fd);
        return 0;
    }

    for (struct ldso *dso = head; dso; dso = dso->next)
        if (!dso->kernel_mapped && dso->dev == st.st_dev && dso->ino == st.st_ino)
        {
            close(fd);
            return dso;
        }

    struct ldso temp_dso = {0};
    temp_dso.name = fullname;
    void *map = map_library(fd, &temp_dso);
    close(fd);
    if (!map) return 0;

    if (find_sym("stdin", &temp_dso).sym && find_sym("_Exit", &temp_dso).sym)
    {
        munmap(temp_dso.map, temp_dso.map_len);
        return load_libc(0);
    }

    struct ldso *dso = malloc(sizeof (struct ldso) + strlen(fullname) + 1);
    if (!dso) {
        print_error("Error loading `%s': Out of memory", name);
        munmap(temp_dso.map, temp_dso.map_len);
    }

    *dso = temp_dso;
    dso->name = (void *)(dso + 1);
    dso->dev = st.st_dev;
    dso->ino = st.st_ino;
    strcpy((char *)dso->name, fullname);
    if (fullname != name) dso->shortname = strrchr(dso->name, '/') + 1;
    dso->prev = tail;
    tail->next = dso;
    tail = dso;

    if (ldd_mode) dprintf(1, "\t%s%s%s (0x%0*tx)\n", dso->shortname? dso->shortname : "", dso->shortname? " => " : "", dso->name, 2 * sizeof (size_t), dso->base);
    return 0;
}

static void load_preload(char *preload, const char *search_path)
{
    char *a, *z;
    a = preload;
    for (;;)
    {
        z = a + __stridx(a, ':');
        int cont = *z;
        *z = 0;
        load_library(a, search_path);
        if (!cont) break;
        *z = ':';
        a = z + 1;
    }
}

static void load_direct_deps(struct ldso *dso, const char *search_path)
{
    if (dso->deps) return;
    size_t cnt = 0;
    /* preload libraries are pseudo-deps of the main application
     * for the purposes of initializing and stuff.
     */
    if (dso == &main) {
        for (struct ldso *ld = dso->next; ld; ld = ld->next)
            cnt++;
    }
    for (const size_t *i = dso->dynv; *i; i += 2)
        if (*i == DT_NEEDED)
            cnt++;

    if (!cnt) {
        dso->deps = no_deps;
        return;
    }

    struct ldso **deps;
    if (dso == &main && cnt < 2)
        deps = builtin_deps;
    else {
        deps = calloc(cnt + 1, sizeof (struct ldso *));
        if (!deps) {
            print_error("Error loading `%s': Could not allocate space for dependencies.", dso->name);
            return;
        }
    }

    cnt = 0;
    if (dso == &main)
        for (struct ldso *ld = dso->next; ld; ld = ld->next)
            deps[cnt++] = ld;
    for (const size_t *i = dso->dynv; *i; i += 2)
        if (i[0] == DT_NEEDED)
            deps[cnt++] = load_library(dso->strtab + i[1], search_path);
    deps[cnt] = 0;
    dso->deps = deps;
}

static void load_deps(struct ldso *dso, const char *search_path)
{
    for (; dso; dso = dso->next)
        load_direct_deps(dso, search_path);
}

static _Noreturn void setup_tmp_threadptr(long *sp, const size_t *dynv);
static _Noreturn void load_run_remaining(long *sp, const size_t *dynv, size_t *aux);

_Noreturn void _start_c(long *sp, const size_t *dynv, long base)
{
    /* first stage (rcrt1s.S) already processed relative relocs.
     * Therefore we can already use local functions and data.
     * We need to initialize the dynlinker enough to be able to process our symbolic relocs.
     * Then do so and go to the next stage.
     */
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
        if (R_TYPE(rel[i+1]) != REL_RELATIVE)
            relsymcnt++;

    /* there shouldn't be too many like that, so if we do have a large number of them,
     * just crash.
     */
    if (relsymcnt > 4096) a_crash();

    size_t original_addends[relsymcnt? relsymcnt : 1];
    for (size_t i = 0, left = relsz, j = 0; left; i++, left -= 2 * sizeof (size_t))
        if (R_TYPE(rel[2*i+1]) != REL_RELATIVE)
            original_addends[j++] = *(size_t *)(base + rel[2*i]);

    libc_original_addends = original_addends;

    head = &libc;
    libc.base = base;
    libc.dynv = dynv;
    libc.name = libc.shortname = "libc.so";
    libc.kernel_mapped = 1;
    process_dynv(&libc);

    relocate(&libc);
    libc.relocated = 0;

    void (*next_stage)(long *sp, const size_t *dynv) = setup_tmp_threadptr;
    __asm__ volatile("" : "+r"(next_stage) :: "memory");
    next_stage(sp, dynv);
    for (;;);
}

#define AUX_CNT (AT_SYSINFO + 1)
static _Noreturn void setup_tmp_threadptr(long *sp, const size_t *dynv)
{
    /* we set up a temporary thread pointer to use for holding errno, hwcap, and sysinfo */
    /* also, cause it's on the way, we set __environ for use with getenv() later. */
    size_t aux[AUX_CNT] = {0};
    struct __pthread tp = {0};
    tp.self = tp.next = tp.prev = &tp;
    size_t *auxv = (void *)(sp + 2 + *sp);
    __environ = (void *)auxv;
    while (*auxv++);
    decode_vec(aux, auxv, AUX_CNT);
    __page_size = aux[AT_PAGESZ];
    __elevated = (aux[0] & 0x7800) != 0x7800 || aux[AT_UID] != aux[AT_EUID] || aux[AT_GID] != aux[AT_EGID] || aux[AT_SECURE];
    if (aux[AT_SYSINFO]) __sysinfo = aux[AT_SYSINFO];
    tp.sysinfo = aux[AT_SYSINFO];
    tp.hwcap = aux[AT_HWCAP];
    if (__set_thread_area(__tp_adjust(&tp)))
        a_crash();

    void (*next_stage)(long *sp, const size_t *dynv, size_t *aux) = load_run_remaining;
    __asm__ volatile("" : "+r"(next_stage) :: "memory");
    next_stage(sp, dynv, aux);
    for (;;);
}

static _Noreturn void load_run_remaining(long *sp, const size_t *dynv, size_t *aux)
{
    int argc = *sp;
    char **argv = (void *)(sp + 1);

    char *lib_path = 0;
    char *preload = 0;

    if (!__elevated)
    {
        lib_path = getenv("LD_LIBRARY_PATH");
        preload = getenv("LD_PRELOAD");
    }
    if (aux[AT_BASE]) {
        /* we were called as dynamic linker */
        main.kernel_mapped = 1;
        main.name = (void *)aux[AT_EXECFN];
        main.shortname = argv[0];
        Phdr *ph0 = (void *)aux[AT_PHDR];
        Phdr *ph = ph0;
        size_t interp_off = 0;
        size_t dyn_off = 0;
        for (size_t i = 0; i < aux[AT_PHNUM]; i++, ph = (void *)((char *)ph + aux[AT_PHENT])) {
            if (ph->p_type == PT_PHDR) {
                main.base = aux[AT_PHDR] - ph->p_vaddr;
            } else if (ph->p_type == PT_INTERP) {
                interp_off = ph->p_vaddr;
            } else if (ph->p_type == PT_DYNAMIC) {
                dyn_off = ph->p_vaddr;
            }
        }
        if (interp_off) libc.name = (void *)(main.base + interp_off);
        if (dyn_off) {
            main.dynv = (void *)(main.base + dyn_off);
            process_dynv(&main);
        }
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
        argv[-1] = (void *)(intptr_t)argc;
        int fd = open(argv[0], O_RDONLY);
        if (fd == -1) {
            print_error("could not load `%s': %m", argv[0]);
            _Exit(1);
        }
        struct stat st;
        if (fstat(fd, &st)) {
            print_error("could not stat `%s': %m", argv[0]);
            _Exit(1);
        }
        main.name = main.shortname = argv[0];
        main.dev = st.st_dev;
        main.ino = st.st_ino;
        Ehdr *eh = map_library(fd, &main);
        if (!eh) _Exit(1);
        close(fd);
        aux[AT_ENTRY] = main.base + eh->e_entry;
    }

    if (!lib_path) lib_path = "/lib:/usr/lib";

    head = tail = &main;

    if (preload) load_preload(preload, lib_path);
    load_deps(head, lib_path);

    reloc_all(main.next);
    relocate(&main);

    if (early_error) _Exit(1);
    if (ldd_mode) _Exit(0);

    runtime = 1;
    a_stackjmp((void *)aux[AT_ENTRY], argv - 1);
    for (;;);
}

