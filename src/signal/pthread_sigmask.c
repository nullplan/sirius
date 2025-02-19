#include <signal.h>
#include "syscall.h"

int pthread_sigmask(int act, const sigset_t *restrict ss, sigset_t *restrict oss)
{
    return -__syscall(SYS_rt_sigprocmask, act, ss, oss, _NSIG/8);
}
