#include <setjmp.h>
#include <signal.h>
#include "syscall.h"

hidden int __sigsetjmp_tail(sigjmp_buf jb, int val)
{
    syscall(SYS_rt_sigprocmask, SIG_SETMASK, val? &jb->__ss : 0, val? 0 : &jb->__ss, _NSIG/8);
    return val;
}
