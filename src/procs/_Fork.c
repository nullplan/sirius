#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include "syscall.h"
#include "libc.h"

pid_t _Fork(void)
{
    sigset_t ss;
    __block_all_sigs(&ss);
    #ifdef SYS_fork
    pid_t rv = syscall(SYS_fork);
    #else
    pid_t rv = syscall(SYS_clone, SIGCHLD, 0);
    #endif
    if (!rv) {
        pthread_t self = __pthread_self();
        self->next = self->prev = self;
        self->tid = __syscall(SYS_set_tid_address, &__thread_list_lock);
        __threaded = 0;
        __thread_list_lock = 0;
    }
    __restore_sigs(&ss);
    return rv;
}
