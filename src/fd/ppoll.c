#include <poll.h>
#include <signal.h>
#include <limits.h>
#include "syscall.h"

int ppoll(struct pollfd pfd[], nfds_t nfd, const struct timespec *restrict ts, const sigset_t *restrict ss)
{
    #ifdef SYS_poll
    if (!ss && (!ts || (ts->tv_sec < INT_MAX / 1000 && ts->tv_nsec < 1000000000ul))) {
        int to = -1;
        if (ts) to = ts->tv_sec * 1000 + (ts->tv_nsec + 999999) / 1000000;
        return syscall_cp(SYS_poll, pfd, nfd, to);
    }
    #endif
    int rv;
    if (ts && ts->tv_nsec >= 1000000000ul)
        rv = -EINVAL;
    else {
        struct timespec lts;
        if (ts) {
            lts = *ts;
            #ifndef __SIXTY_FOUR
            lts->__pad = 0;
            #endif
            ts = &lts;
        }
        rv = __syscall_cp(SYS_ppoll, pfd, nfd, ts, ss, _NSIG/8);
    }
    return __syscall_ret(rv);
}
