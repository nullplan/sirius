#include <sys/auxv.h>
#include "libc.h"

hidden unsigned long __getauxval(unsigned long tag)
{
    for (const size_t *p = __auxv; *p; p += 2)
        if (p[0] == tag)
            return p[1];
    return 0;
}
weak_alias(getauxval, __getauxval);
