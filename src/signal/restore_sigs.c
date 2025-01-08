#include <signal.h>
#include "syscall.h"

hidden int __restore_sigs(const sigset_t *ss)
{
    return __syscall(SYS_rt_sigprocmask, SIG_SETMASK, ss, 0, _NSIG/8);
}
