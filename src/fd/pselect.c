#include <sys/select.h>
#include "syscall.h"

int pselect(int width, fd_set *restrict rfd, fd_set *restrict wfd, fd_set *restrict xfd, const struct timespec *restrict ts, const sigset_t *restrict ss)
{
    struct timespec lts;
    if (ts) {
        lts = *ts;
        ts = &lts;
    }
    return syscall(SYS_pselect6, width, rfd, wfd, xfd, ts, ((long[]){(long)ss, _NSIG/8}));
}
