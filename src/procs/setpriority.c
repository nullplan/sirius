#include <sys/resource.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include "syscall.h"
#include "cpu.h"

int setpriority(int which, id_t who, int prio)
{
    prio = NZERO - prio;
    int rv = syscall(SYS_setpriority, which, who, prio);
    if (rv || which != PRIO_PROCESS || (who != 0 && who != __syscall(SYS_getpid)))
        return rv;

    sigset_t ss;
    __block_usr_sigs(&ss);
    __tl_lock();
    pthread_t self = __pthread_self();
    for (pthread_t t = self->next; t != self; t = t->next)
        if (t->tid && __syscall(SYS_setpriority, t->tid, PRIO_PROCESS, prio))
            a_crash();
    __tl_unlock();
    __restore_sigs(&ss);
    return 0;
}
