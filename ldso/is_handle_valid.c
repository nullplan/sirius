#include "ldso.h"
hidden int __dl_is_handle_valid(void *h)
{
    for (struct dso *dso = __dl_head(); dso; dso = dso->next)
        if (dso == h)
            return 1;
    return 0;
}

