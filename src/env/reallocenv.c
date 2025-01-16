#include <stdlib.h>
#include <string.h>

hidden int __reallocenv(size_t new)
{
    static char **allocenv;
    char **p;
    if (__environ && __environ == allocenv)
        p = realloc(allocenv, new * sizeof (char *));
    else {
        p = malloc(new * sizeof (char *));
        if (p && __environ)
            memcpy(p, __environ, (new - 1) * sizeof (char *));
    }
    if (p) {
        __environ = allocenv = p;
        return 0;
    }
    return -1;
}
