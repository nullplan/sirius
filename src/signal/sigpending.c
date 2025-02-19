#include <signal.h>
#include "syscall.h"

int sigpending(sigset_t *ss)
{
    return syscall(SYS_rt_sigpending, ss, _NSIG/8);
}
