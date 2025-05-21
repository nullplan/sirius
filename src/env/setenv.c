#include <stdlib.h>
#include <string.h>
#include <errno.h>

hidden void __add_rm_env_map(char *add, char *rm)
{
    static char **envmap;
    static size_t mapc;

    if (rm) {
        for (size_t i = 0; i < mapc; i++)
            if (envmap[i] == rm) {
                free(rm);
                envmap[i] = add;
                return;
            }
    }

    if (add) {
        for (size_t i = 0; i < mapc; i++)
            if (!envmap[i]) {
                envmap[i] = add;
                return;
            }
        size_t attempt = mapc < 11? 16 : mapc + mapc/2;
        char **p = realloc(envmap, attempt * sizeof (char *));
        if (!p) return;
        envmap = p;
        envmap[mapc] = add;
        for (size_t i = mapc + 1; i < attempt; i++)
            envmap[i] = 0;
        mapc = attempt;
    }
}

int setenv(const char *name, const char *value, int overwrite)
{
    size_t namelen = __stridx(name, '=');
    if (!namelen || name[namelen]) {
        errno = EINVAL;
        return -1;
    }

    size_t envc = 0;
    char **p;
    if (__environ) {
        p = __lookup_env(name, namelen);
        if (*p && !overwrite) return 0;
    }

    size_t valuelen = strlen(value);
    char *str = malloc(namelen + valuelen + 2);
    if (!str) return -1;
    memcpy(str, name, namelen);
    str[namelen] = '=';
    memcpy(str + namelen + 1, value, valuelen + 1);

    if (__environ) {
        if (*p) {
            __add_rm_env_map(str, *p);
            *p = str;
            return 0;
        }
        else
            envc = p - __environ;
    }

    if (__reallocenv(envc + 2)) {
        free(str);
        return -1;
    }

    __add_rm_env_map(str, 0);
    __environ[envc] = str;
    __environ[envc + 1] = 0;
    return 0;
}
