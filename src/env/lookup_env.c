#include <stdlib.h>
#include <string.h>
#include <assert.h>

hidden char **__lookup_env(const char *name, size_t namelen)
{
    assert(__environ);
    char **p;
    for (p = __environ; *p; p++)
        if (strncmp(*p, name, namelen) == 0 && namelen[*p] == '=')
            break;
    return p;
}
