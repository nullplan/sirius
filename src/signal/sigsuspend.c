#include <signal.h>
#include "syscall.h"

int sigsuspend(const sigset_t *ss)
{
    return syscall_cp(SYS_rt_sigsuspend, ss, _NSIG/8);
}
