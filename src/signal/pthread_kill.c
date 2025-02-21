#include <signal.h>
#include <pthread.h>
#include "syscall.h"

int pthread_kill(pthread_t t, int sig)
{
    if (t == __pthread_self())
        return -__syscall(SYS_tkill, t->tid, sig);
    sigset_t ss;
    __block_all_sigs(&ss);
    __lock(&t->killlock);
    int rv = 0;
    if (t->tid)
        rv = -__syscall(SYS_tkill, t->tid, sig);
    __unlock(&t->killlock);
    __restore_sigs(&ss);
    return rv;
}
