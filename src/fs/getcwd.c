#include <unistd.h>
#include <string.h>
#include <limits.h>
#include "syscall.h"

static char *inner_getcwd(char *p, size_t len)
{
    long rv = __syscall(SYS_getcwd, p, len);
    if (rv > -4096ul) {
        errno = -rv;
        return 0;
    }
    if (*p != '/') {
        errno = ENOENT;
        return 0;
    }
    return p;
}
char *getcwd(char *p, size_t len)
{
    char buf[p? 1 : PATH_MAX];
    if (!p) {
        p = inner_getcwd(buf, PATH_MAX);
        if (!p) return 0;
        return strdup(p);
    }
    return inner_getcwd(p, len);
}
