#include <sys/statvfs.h>
#include "syscall.h"

int statvfs(const char *restrict path, struct statvfs *restrict s)
{
    int rv = syscall(SYS_statfs, path, s);
    if (!rv)
        s->f_favail = s->f_ffree;
    return rv;
}
