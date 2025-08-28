#include <pthread.h>
#include <signal.h>
#include "syscall.h"

int pthread_setschedprio(pthread_t t, int prio)
{
    int rv = ESRCH;
    sigset_t ss;
    __block_all_sigs(&ss);
    __lock(&t->killlock);
    if (t->tid)
        rv = -__syscall(SYS_sched_setparam, t->tid, &prio);
    __unlock(&t->killlock);
    __restore_sigs(&ss);
    return rv;
}
