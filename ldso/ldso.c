#include <stddef.h>
#include <stdint.h>
#include <elf.h>
#include "cpu.h"

#ifndef __SIXTY_FOUR
typedef Elf32_Sym Sym;
#define R_SYM(i) ELF32_R_SYM(i)
#define R_TYPE(i) ELF32_R_TYPE(i)
#else
typedef Elf64_Sym Sym;
#define R_SYM(i) ELF64_R_SYM(i)  ((i)>>32)
#define R_TYPE(i) ELF64_R_TYPE(i) ((i)&0xffffffffU)
#endif
struct ldso {
    uintptr_t base;
    const size_t *dynv;
    const uint32_t *hashtab;
    const uint32_t *ghashtab; /* XXX: check type */
    const Sym *symtab;
    const char *strtab;
    int relocated;
    struct ldso *next;
};

static struct ldso main, libc, vdso;
static struct ldso *head;
static const size_t *libc_original_addends;

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

static void decode_vec(size_t *arr, const size_t *vec, size_t lim)
{
    for (; *vec; vec += 2)
        if (*vec < lim)
            arr[vec[0]] = vec[1];
}

static _Noreturn void load_run_remaining(long *sp, const size_t *dynv)
{
}

static _Noreturn void setup_tmp_threadptr(long *sp, const size_t *dynv)
{
    void (*next_stage)(long *sp, const size_t *dynv) = load_run_remaining;
    __asm__ volatile("" : "+r"(next_stage) :: "memory");
    next_stage(sp, dynv);
    for (;;);
}

#define DYN_CNT (DT_RELSZ + 1)
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

    size_t dyn[DYN_CNT] = {0};
    decode_vec(dyn, dynv, DYN_CNT);
    process_dynv(&libc);

    /* we need to perform symbolic relocation now,
     * but we need to re-perform it after loading all other libs.
     * This works with DT_RELA, but not with DT_REL,
     * so we need to save the original addends first.
     *
     * To that end we count the symbolic DT_REL relocations.
     */
    size_t relsymcnt = 0;
    const size_t *rel = (void *)(base + dyn[DT_REL]);
    for (size_t i = 0, left = dyn[DT_RELSZ]; left; i += 2, left -= 2 * sizeof (size_t))
    {
        if (R_TYPE(rel[i+1]) != R_X86_64_RELATIVE)
            relsymcnt++;
    }

    /* there shouldn't be too many like that, so if we do have a large number of them,
     * just crash.
     */
    if (relsymcnt > 4096) a_crash();

    size_t original_addends[relsymcnt? relsymcnt : 1];
    for (size_t i = 0, left = dyn[DT_RELSZ], j = 0; left; i++, left -= 2 * sizeof (size_t))
    {
        if (R_TYPE(rel[2*i+1]) != R_X86_64_RELATIVE)
            original_addends[j++] = *(size_t *)(base + rel[2*i]);
    }

    libc_original_addends = original_addends;
    reloc_all(&libc);
    libc.relocated = 0;

    void (*next_stage)(long *sp, const size_t *dynv) = setup_tmp_threadptr;
    __asm__ volatile("" : "+r"(next_stage) :: "memory");
    next_stage(sp, dynv);
    for (;;);
}
