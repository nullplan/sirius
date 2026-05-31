#include <dlfcn.h>
#include "ldso.h"

int dlclose(void *h)
{
    int rv = 0;
    __dl_rdlock();
    if (!__dl_is_handle_valid(h)) {
        __dl_runtime_err("invalid handle");
        rv = -1;
    }
    __dl_unlock();
    return rv;
}
