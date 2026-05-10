#include "ldso.h"
#include <dlfcn.h>
#include <elf.h>
#include <pthread.h>

hidden void *__dlsym(void *restrict handle, const char *restrict name, uintptr_t caller)
{
    struct dso *h = handle;
    __dl_rdlock();
    if (handle == RTLD_DEFAULT)
        h = __dl_head();
    else if (handle == RTLD_NEXT) {
        h = __dl_head();
        for (struct dso *dso = h; dso; dso = dso->next)
        {
            if (caller - (uintptr_t)dso->map < dso->map_len) {
                h = dso->next;
                break;
            }
        }
    }
    struct symdef def = __dl_find_sym(name, h, 1);
    void *rv = 0;
    if (!def.dso)
        __dl_runtime_err("Symbol `%s' not found", name);
    else if ((def.sym->st_info & 15) == STT_TLS)
        rv = __tls_get_addr((size_t[]){def.dso->tlsid, def.sym->st_value - DTV_OFFSET});
    else
        rv = (void *)(def.dso->base + def.sym->st_value);
    __dl_unlock();
    return rv;

}
