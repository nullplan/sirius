#include <unistd.h>
#include <string.h>
#include <limits.h>
#include "syscall.h"

char *getcwd(char *p, size_t len)
{
    char buf[p? 1 : PATH_MAX];
    if (!p) {
        p = buf;
        len = PATH_MAX;
    }
    if (syscall(SYS_getcwd, p, len) < 0)
        return 0;
    if (*p != '/') {
        errno = ENOENT;
        return 0;
    }
    return p == buf? strdup(p) : p;
}
