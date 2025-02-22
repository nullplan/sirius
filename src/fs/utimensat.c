#include <sys/stat.h>
#include "syscall.h"

int utimensat(int fd, const char *path, const struct timespec ts[2], int flags)
{
    int rv;
#ifdef SYS_futimesat
    if (!flags)
        rv = __syscall(SYS_futimesat, fd, path, ts);
    else
#endif
        rv = __syscall(SYS_utimensat, fd, path, ts, flags);
    return __syscall_ret(rv);
}
