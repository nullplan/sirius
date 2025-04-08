#include <poll.h>
#include "syscall.h"

int poll(struct pollfd pfd[], nfds_t nfd, int to)
{
    #ifdef SYS_poll
    return syscall(SYS_poll, pfd, nfd, to);
    #else
    struct timespec ts, *pts = 0;
    if (to >= 0) {
        ts = (struct timespec){
            .tv_sec = to / 1000,
            .tv_nsec = (to % 1000) * 1000000,
        };
        pts = &ts;
    }
    return syscall(SYS_ppoll, pfd, nfd, pts, 0);
    #endif
}
