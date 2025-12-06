#define __STARTUP_CODE
#include "ldso.h"
#include "libc.h"
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

static size_t *libc_original_addends;
static struct dso libc;
hidden size_t __libc_addend(size_t idx)
{
    return libc_original_addends[idx];
}
hidden struct dso *__libc_object(void)
{
    return &libc;
}

static void early_process_relocs(const size_t *rel, size_t relsz, size_t stride)
{
    size_t addend_idx = 0;
    for (; relsz; rel += stride, relsz -= stride * sizeof (size_t))
    {
        int type = R_TYPE(rel[1]);
        if (!type || type == REL_RELATIVE) continue;
        size_t *rel_addr = (void *)(libc.base + rel[0]);
        size_t addend;
        if (stride == 3)
            addend = rel[2];
        else if (type == REL_PLT || type == REL_GOT)
            addend = 0;
        else {
            addend = *rel_addr;
            libc_original_addends[addend_idx++] = addend;
        }

        size_t symval = 0;
        if (R_SYM(rel[1])) {
            const Sym *sym = libc.symtab + R_SYM(rel[1]);
            /* libc has, at least initially, all symbols resolved internally.
             * Therefore, no symbol referenced by a relocation can be undefined,
             * and if it is, then that's an error
             */
            if (!sym->st_shndx) {
                const char *msg[3];
                msg[0] = "Error relocating libc: Symbol `";
                msg[1] = libc.strtab + sym->st_name;
                msg[2] = "' undefined\n";
                __die_early_multi(msg, 3);
            }
            symval = (uintptr_t)(libc.base + sym->st_value);
        }
        switch (type)
        {
            case REL_PLT:
            case REL_GOT:
            case REL_SYMBOLIC:
                *rel_addr = symval + addend;
                break;

            case REL_USYMBOLIC:
                memcpy(rel_addr, &(size_t){symval + addend}, sizeof (size_t));
                break;

            case REL_PCREL32:
                *(uint32_t *)rel_addr = symval + addend - (uintptr_t)rel_addr;
                break;

            default:
                {
                    char typestr[3 * sizeof (int) + 2];
                    int neg = 0;
                    unsigned x = type;
                    char *p = typestr + sizeof typestr;
                    *--p = 0;
                    if (type < 0) {
                        x = -type;
                        neg = 1;
                    }
                    do *--p = '0' + x % 10;
                    while (x /= 10);
                    if (neg) *--p = '-';
                    const char *msg[3];
                    msg[0] = "Error relocating libc: Unknown/invalid relocation type ";
                    msg[1] = p;
                    msg[2] = "\n";
                    __die_early_multi(msg, 3);
                }
                break;
        }
    }
}

static _Noreturn void setup_tmp_threadptr(long *sp, const size_t *dynv);
hidden _Noreturn void _start_c(long *sp, const size_t *dynv, long base)
{
    /* first stage (rcrt1s.S) already processed relative relocs.
     * Therefore we can already use local functions and data.
     * We need to initialize the dynlinker enough to be able to process our symbolic relocs.
     * Then do so and go to the next stage.
     */
    libc.base = (void *)base;
    libc.dynv = dynv;
    libc.name = libc.shortname = "libc.so";
    libc.kernel_mapped = 1;
    const Ehdr *eh = (void *)base;
    libc.phdr = (void *)(libc.base + eh->e_phoff);
    libc.phent = eh->e_phentsize;
    libc.phnum = eh->e_phnum;
    size_t relsz = 0;
    size_t relsymcnt = 0;
    size_t relasz = 0;
    const size_t *rel = 0;
    const size_t *rela = 0;
    for (const size_t *d = dynv; *d; d += 2)
    {
        void *p = libc.base + d[1];
        switch (*d) {
            case DT_STRTAB:     libc.strtab = p; break;
            case DT_SYMTAB:     libc.symtab = p; break;
            case DT_REL:        rel = p; break;
            case DT_RELSZ:      relsz = d[1]; break;
            case DT_RELA:       rela = p; break;
            case DT_RELASZ:     relasz = d[1]; break;
        }
    }
    if (!libc.strtab || !libc.symtab)
        __die_early("Error relocating libc: Missing vital data.\n");

    /* we need to perform symbolic relocation now,
     * but we need to re-perform it after loading all other libs.
     * This works with DT_RELA, but not with DT_REL,
     * so we need to save the original addends first.
     *
     * To that end we count the symbolic DT_REL relocations.
     */
    for (size_t i = 0, left = relsz; left; i += 2, left -= 2 * sizeof (size_t))
        if (R_TYPE(rel[i+1]) != REL_RELATIVE)
            relsymcnt++;

    /* there shouldn't be too many like that, so if we do have a large number of them,
     * just crash.
     */
    if (relsymcnt > 4096) __die_early("Error relocating libc: Too many symbolic rels.\n");

    size_t original_addends[relsymcnt + 1];
    libc_original_addends = original_addends;

    if (relsz) early_process_relocs(rel, relsz, 2);
    if (relasz) early_process_relocs(rela, relasz, 3);

    /* compiler barrier to avoid hoisting any code into an earlier stage. */
    void (*next_stage)(long *sp, const size_t *dynv) = setup_tmp_threadptr;
    __asm__ volatile("" : "+r"(next_stage) :: "memory");
    next_stage(sp, dynv);
    __builtin_unreachable();
}

static _Noreturn void setup_tmp_threadptr(long *sp, const size_t *dynv)
{
    /* set up a temporary thread pointer to house the TID, errno, hwcap, and sysinfo. */
    /* TLL doesn't get set up at this point, but is also not necessary. */
    /* __auxv must be set here because ARM __set_thread_area depends on it.
     * __environ is set up because it is on the way
     */
    struct __pthread tp;
    tp.self = tp.next = tp.prev = &tp;
    int argc = *sp;
    char **argv = (void *)(sp + 1);
    char **envp = (void *)(argv + argc + 1);
    const size_t *auxv = (void *)envp;
    while (*auxv++);
    size_t aux[AUX_CNT] = {0};
    __environ = envp;
    __auxv = auxv;
    __decode_vec(aux, auxv, AUX_CNT);
    tp.hwcap = aux[AT_HWCAP];
    tp.sysinfo = __get_sysinfo(aux);
    if (__set_thread_area(__tp_adjust(&tp)))
        __die_early("Could not initialize thread pointer.\n");

    void (*next_stage)(const size_t *, int, char **, char **, size_t *) = __stage_post_reloc;
    __asm__ volatile("": "+r"(next_stage) :: "memory");
    next_stage(dynv, argc, argv, envp, aux);
    __builtin_unreachable();
}
