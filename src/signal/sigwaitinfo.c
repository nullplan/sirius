#include <signal.h>
#include "syscall.h"

int sigwaitinfo(const sigset_t *restrict ss, siginfo_t *restrict si)
{
    return syscall_cp(SYS_rt_sigtimedwait, ss, si, 0, _NSIG/8);
}
