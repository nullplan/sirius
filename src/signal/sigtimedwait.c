#include <signal.h>
#include "syscall.h"

int sigtimedwait(const sigset_t *restrict ss, siginfo_t *restrict si,
        const struct timespec *restrict to)
{
    return syscall(SYS_rt_sigtimedwait, ss, si, to, _NSIG/8);
}
