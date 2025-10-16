#include "ldso.h"
#include <stdlib.h>

static void reclaim_single(const struct dso *dso, size_t start, size_t end)
{
    if (start >= dso->relro_start && start < dso->relro_end)
        start = dso->relro_end;
    if (end >= dso->relro_start && end < dso->relro_end)
        end = dso->relro_start;
    if (start < end)
        __donate_malloc_memory(dso->base + start, end - start);
}

hidden void __reclaim_gaps(const struct dso *dso)
{
    size_t n = dso->phnum;
    for (const Phdr *ph = dso->phdr; n; n--, ph = (void *)((char *)ph + dso->phent))
    {
        if (ph->p_type != PT_LOAD || (ph->p_flags & (PF_R|PF_W)) != (PF_R|PF_W))
            continue;
        reclaim_single(dso, ph->p_vaddr & -PAGE_SIZE, ph->p_vaddr);
        reclaim_single(dso, ph->p_vaddr + ph->p_memsz, (ph->p_vaddr + ph->p_memsz + PAGE_SIZE - 1) & -PAGE_SIZE);
    }
}
