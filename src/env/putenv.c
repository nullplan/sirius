#include <stdlib.h>
#include <string.h>
#include <errno.h>

static void dummy(char *a, char *b) {}
weak_alias(__add_rm_env_map, dummy);

int putenv(char *p)
{
    size_t namelen = __stridx(p, '=');
    if (!p[namelen]) {
        errno = EINVAL;
        return -1;
    }

    size_t envc = 0;
    if (__environ) {
        char **var = __lookup_env(p, namelen);
        if (*var) {
            __add_rm_env_map(0, *var);
            *var = p;
            return 0;
        }

        envc = var - __environ;
    }
    if (__reallocenv(envc + 2)) return -1;
    __environ[envc] = p;
    __environ[envc + 1] = 0;
    return 0;
}
