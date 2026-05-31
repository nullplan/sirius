#include <dlfcn.h>
#include "ldso.h"
#include "vdso.h"

static size_t get_symcnt(const struct dso *dso)
{
    if (dso->hashtab)
        return dso->hashtab[1];
    return __count_ghashtab(dso->ghashtab);
}

int dladdr(const void *restrict addr, Dl_info_t *restrict dli)
{
    int rv = 0;
    __dl_rdlock();
    for (struct dso *dso = __dl_head(); dso; dso = dso->next)
    {
        if ((uintptr_t)addr - (uintptr_t)dso->map < dso->map_len)
        {
            rv = 1;
            dli->dli_fname = dso->name;
            dli->dli_fbase = dso->map;
            dli->dli_sname = 0;
            dli->dli_saddr = 0;
            size_t symcnt = get_symcnt(dso);
            for (const Sym *sym = dso->symtab + 1; sym < dso->symtab + symcnt; sym++)
            {
                if (dso->base + sym->st_value <= (char *)addr && dso->base + sym->st_value > (char *)dli->dli_saddr)
                {
                    dli->dli_saddr = dso->base + sym->st_value;
                    dli->dli_sname = dso->strtab + sym->st_name;
                }
            }
        }
    }
    __dl_unlock();
    return rv;
}
