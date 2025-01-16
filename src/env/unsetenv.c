#include <stdlib.h>
#include <string.h>
#include <errno.h>

static void dummy(char *a, char *b) {}
weak_alias(__add_rm_env_map, dummy);

int unsetenv(const char *name)
{
    size_t namelen = __stridx(name, '=');
    if (!namelen || name[namelen]) {
        errno = EINVAL;
        return -1;
    }

    if (!__environ) return 0;
    char **p = __lookup_env(name, namelen);
    if (*p) {
        __add_rm_env_map(0, *p);
        char **q;
        for (q = p + 1; *q; q++);
        *p = q[-1];
        q[-1] = 0;
    }

    return 0;
}
