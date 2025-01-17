#include <stdlib.h>
#include <string.h>

char *getenv(const char *name)
{
    if (!__environ) return 0;
    size_t namelen = __stridx(name, '=');
    if (name[namelen]) return 0;
    char **p = __lookup_env(name, namelen);
    if (!*p) return 0;
    return *p + namelen + 1;
}
