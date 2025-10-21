#define _GNU_SOURCE
#include <sys/stat.h>
#include <fcntl.h>
#include "syscall.h"

int fchmodat(int dfd, const char *nm, mode_t m, int flags)
{
    int rv;
    #ifdef SYS_chmod
    if ((dfd == AT_FDCWD || *nm == '/') && !flags)
        rv = __syscall(SYS_chmod, nm, m);
    else
    #endif
    if ((flags & AT_EMPTY_PATH) && dfd >= 0 && !*nm)
        rv = __syscall(SYS_fchmod, dfd, m);
    else
        rv = __syscall(SYS_fchmodat, dfd, nm, m, flags);
    return __syscall_ret(rv);
}
