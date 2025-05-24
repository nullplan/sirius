#include <pthread.h>
#include <signal.h>
#include "syscall.h"

int pthread_setschedparam(pthread_t t, int pol, const struct sched_param *prio)
{
    int rv = ESRCH;
    sigset_t ss;
    __block_usr_sigs(&ss);
    __lock(&t->killlock);
    if (t->tid)
        rv = -__syscall(SYS_sched_setscheduler, t->tid, pol, prio);
    __unlock(&t->killlock);
    __restore_sigs(&ss);
    return rv;
}
