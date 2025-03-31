#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include "libc.h"
#include "syscall.h"

static void dummy(int x){}
weak_alias(__atfork_run, dummy);

pid_t fork(void)
{
    __atfork_run(-1);
    sigset_t ss;
    __block_usr_sigs(&ss);
    __tl_lock();
    pthread_t self = __pthread_self();
    pthread_t t = self->next;
    pid_t rv = _Fork();
    if (!rv) {
        for (; t != self; t = t->next) {
            t->tid = 0;
            for (FILE *f = t->locked_files; f; f = f->lnext)
                f->lock = FUTEX_OWNER_DIED;
        }
    }
    __tl_unlock();
    __restore_sigs(&ss);
    __atfork_run(rv == 0);
    return rv;
}
