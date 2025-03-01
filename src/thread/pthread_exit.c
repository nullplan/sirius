#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include "futex.h"
#include "syscall.h"
#include "cpu.h"

static unsigned long exitstack[64];

static _Noreturn void unmapself(void)
{
    pthread_t self = __pthread_self();
    __syscall(SYS_munmap, self->map, self->map_size);
    for (;;)
        __syscall(SYS_exit, 0);
}

static void dummy(void) {}
weak_alias(__pthread_tsd_destroy, dummy);

_Noreturn void pthread_exit(void *result)
{
    pthread_t self = __pthread_self();
    self->cancelstate = PTHREAD_CANCEL_DISABLE;

    while (self->cb) {
        self->cb->__fn(self->cb->__a);
        self->cb = self->cb->__next;
    }

    __pthread_tsd_destroy();

    sigset_t ss;
    /* here comes the two-stage signal block.
     * I must block all signals eventually, as I cannot continue execution shortly.
     * But I must not hold the thread list lock while user signals are unblocked,
     * and I must not block SIGSYNCCALL while not holding the TL lock.
     * Therefore, this:
     */
    __block_usr_sigs(&ss);
    __tl_lock();
    __block_all_sigs(0);

    /* also, don't go looking for the unlock too hard: The kernel unlocks the TL lock
     * once the thread is so dead it will never execute user code again.
     */

    /* set the detach state after taking the TL lock. That way, if another thread
     * observes detachstate == DT_EXITING, it can be confident the thread has
     * exited once it observes the TL lock being free after that.
     *
     * This must also be an atomic, since this function might race with
     * pthread_detach(), and the loser of that race has to clean up.
     */
    int ds = a_cas(&self->detachstate, DT_JOINABLE, DT_EXITING);

    /* I cannot notice I am the last thread before having taken the TL lock */
    if (self->next == self) {
        /* this means this is the last thread.
         * So now we put everything back the way it was and then exit */
        self->detachstate = ds;
        __tl_unlock();
        __restore_sigs(&ss);
        exit(0);
    }

    __lock(&self->killlock);
    self->tid = 0;
    __unlock(&self->killlock);

    /* kill the locks of all held files, making them impossible to get and deadlock all the time. */
    for (FILE *f = self->locked_files; f; f = f->lnext)
        f->lock = FUTEX_OWNER_DIED;

    /* XXX: Once the robust list becomes a thing, we may want to process it here! */

    /* if we are detached, we have to clean up ourselves.
     * Except C really needs a stack to function, and after the munmap() call,
     * the stack might be gone. Therefore, we transition to a different stack.
     *
     * The exit stack is protected by the thread list lock, so only one thread
     * can execute on it at any time.
     */
    if (ds == DT_DETACHED && self->map)
        a_stackinvoke(unmapself, exitstack + sizeof exitstack / sizeof *exitstack);

    self->result = result;
    __futex_wake(&self->detachstate, 1, 1);
    for (;;)
        __syscall(SYS_exit, 0);
}
