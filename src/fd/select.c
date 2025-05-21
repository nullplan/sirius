#include <sys/select.h>
#include "syscall.h"

int select(int width, fd_set *restrict rfd, fd_set *restrict wfd, fd_set *restrict xfd, struct timeval *restrict tv)
{
#ifdef SYS_select
    return syscall_cp(SYS_select, width, rfd, wfd, xfd, tv);
#else
    struct timespec ts, *pts = 0;
    if (tv) {
        ts.tv_sec = tv->tv_sec + tv->tv_usec / 1000000;
        ts.tv_nsec = (tv->tv_usec % 1000000) * 1000;
        pts = &ts;
    }
    return syscall_cp(SYS_pselect6, width, rfd, wfd, xfd, pts, 0);
#endif
}
