#include <pthread.h>
#include <limits.h>
#include <assert.h>
#include "errno.h"
#include "futex.h"
#include "cpu.h"
#include "syscall.h"
#include "libc.h"

static int futex_unlock_pi(volatile int *fut, int priv)
{
    int op = FUTEX_UNLOCK_PI;
    if (priv) priv = FUTEX_PRIVATE_FLAG;
    int rv = __syscall(SYS_futex, fut, op | priv);
    if (rv == -ENOSYS && priv)
        rv = __syscall(SYS_futex, fut, op);
    return rv;
}

hidden int __pthread_mutex_unlock(pthread_mutex_t *m)
{
    if ((m->__flg & 3) == PTHREAD_MUTEX_RECURSIVE && m->__ctr) {
        m->__ctr--;
        return 0;
    }
    if (!(m->__flg & ~PTHREAD_PROCESS_SHARED)) {
        int priv = !(m->__flg & PTHREAD_PROCESS_SHARED);
        int waiters = m->__waiters;
        if ((a_swap(&m->__lock, 0) & FUTEX_WAITERS) || waiters)
            __futex_wake(&m->__lock, priv, 1);
        return 0;
    }

    pthread_t self = __pthread_self();
    int pi = m->__flg & PTHREAD_PRIO_INHERIT;
    int robust = m->__flg & PTHREAD_MUTEX_ROBUST;
    int mask = robust? FUTEX_TID_MASK : FUTEX_NR_TID_MASK;
    int v = m->__lock;

    /* POSIX specifies this only for errorcheck, recursive, or robust mutexes.
     * Technically not for default PI mutexes. But for those, this is undefined behavior.
     * So may as well.
     */
    if ((v & mask) != self->tid)
        return EPERM;

    self->robust.pending = m;
    if (pi) {
        if (m->__waiters == 1) {
            assert(robust); /* if non-robust, lock can't be acquired, so TID check fails, so we never get here. */
            m->__waiters = 2;
        }
        if (a_cas(&m->__lock, self->tid, 0) != self->tid) {
            futex_unlock_pi(&m->__lock, !(m->__flg & PTHREAD_PROCESS_SHARED));
        }
    } else {
        int w = m->__waiters;
        if (v & FUTEX_OWNER_DIED) {
            assert(robust); /* if non-robust, OWNER_DIED is part of TID mask, so TID check fails, so we never get here. */
            if ((a_swap(&m->__lock, FUTEX_NR_TID_MASK) & FUTEX_WAITERS) || w)
                __futex_wake(&m->__lock, !(m->__flg & PTHREAD_PROCESS_SHARED), INT_MAX); /* broadcast wake because the lock is dead and everyone can leave. */
        } else {
            if ((a_swap(&m->__lock, 0) & FUTEX_WAITERS) || w)
                __futex_wake(&m->__lock, !(m->__flg & PTHREAD_PROCESS_SHARED), 1);
        }
    }

    if (m->__next != (void *)&self->robust.head)
        m->__next->__prev = m->__prev;
    if (m->__prev) m->__prev->__next = m->__next;
    else self->robust.head = m->__next;
    self->robust.pending = 0;
    return 0;
}
weak_alias(pthread_mutex_unlock, __pthread_mutex_unlock);
