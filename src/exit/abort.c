#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include "syscall.h"
#include "cpu.h"

_Noreturn void abort(void)
{
    raise(SIGABRT);
    __block_usr_sigs(0);
    __lock(&__abort_lock);
    __libc_sigaction(SIGABRT, &(struct sigaction){.sa_handler = SIG_DFL}, 0);
    __syscall(SYS_tkill, __pthread_self()->tid, SIGABRT);
    long mask[4] ={0};
    mask[(SIGABRT-1)/(8*sizeof (long))] |= 1ul << ((SIGABRT-1)%(8*sizeof (long)));
    __syscall(SYS_rt_sigprocmask, SIG_UNBLOCK, mask, 0, _NSIG/8);

    /* if we get here, things are now completely undefined. */
    for (;;) {
        __syscall(SYS_exit_group, 1);
        __syscall(SYS_exit, 1);
    }
}
