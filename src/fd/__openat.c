#include <fcntl.h>
#include "syscall.h"

hidden int __openat(int dfd, const char *name, int flags, mode_t m)
{
    int rv;
    flags |= O_LARGEFILE;
#ifdef SYS_open
    if (dfd == AT_FDCWD || *name == '/')
        rv = __syscall(SYS_open, name, flags, m);
    else
#endif
        rv = __syscall(SYS_openat, dfd, name, flags, m);
    if (rv >= 0 && (flags & O_CLOEXEC))
        __syscall(SYS_fcntl, rv, F_SETFD, FD_CLOEXEC);
    return __syscall_ret(rv);
}
