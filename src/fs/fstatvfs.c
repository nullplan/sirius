#include <sys/statvfs.h>
#include "syscall.h"

int fstatvfs(int fd, struct statvfs *sf)
{
    int rv = syscall(SYS_fstatfs, fd, sf);
    if (!rv)
        sf->f_favail = sf->f_ffree;
    return rv;
}
