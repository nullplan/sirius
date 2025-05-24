#include <pthread.h>
#include <signal.h>
#include "syscall.h"

int pthread_getschedparam(pthread_t t, int *restrict pol, struct sched_param *restrict prio)
{
    sigset_t oss;
    __block_usr_sigs(&oss);
    __lock(&t->killlock);
    int rv = ESRCH; /* not quite correct, but the best I can do right now. */
    if (t->tid)
    {
        rv = -__syscall(SYS_sched_getparam, t->tid, prio);
        if (!rv)
            *pol = __syscall(SYS_sched_getscheduler, t->tid);
    }
    __unlock(&t->killlock);
    __restore_sigs(&oss);
    return rv;
}
