#include <pthread.h>
#include <limits.h>
#include <stddef.h>
#include <assert.h>
#include "cpu.h"
#include "futex.h"
#include "libc.h"
#include "syscall.h"

static int futex_pi(volatile int *fut, int priv, int op)
{
    if (priv) priv = FUTEX_PRIVATE_FLAG;
    int rv;
    do {
        rv = __syscall(SYS_futex, fut, op | priv);
        if (rv == -ENOSYS && priv)
            rv = __syscall(SYS_futex, fut, op);
    } while (rv == -EINTR || rv == -EAGAIN);
    return rv;
}

hidden int __pthread_mutex_trylock(pthread_mutex_t *m)
{
    if (!(m->__flg & ~PTHREAD_PROCESS_SHARED)) {
        return a_cas(&m->__lock, 0, EBUSY) & FUTEX_NR_TID_MASK;
    }

    pthread_t self = __pthread_self();
    int mask = m->__flg & PTHREAD_MUTEX_ROBUST? FUTEX_TID_MASK : FUTEX_NR_TID_MASK;
    /* handle re-lock case */
    if ((m->__lock & mask) == self->tid) {
        switch (m->__flg & 3) {
            case PTHREAD_MUTEX_ERRORCHECK:
                return EDEADLK;
            case PTHREAD_MUTEX_RECURSIVE:
                if (m->__ctr == UINT_MAX)
                    return EAGAIN;
                m->__ctr++;
                return 0;

            case PTHREAD_MUTEX_NORMAL:
                return EBUSY;
        }
    }

    if (!self->robust.off) {
        self->robust.head = (void *)&self->robust.head;
        self->robust.off = offsetof(struct __mtx, __lock);
        self->robust.pending = 0;
        __syscall(SYS_set_robust_list, &self->robust, sizeof self->robust);
    }

    int rv = -1;
    self->robust.pending = m;
    if (m->__flg & PTHREAD_PRIO_INHERIT) {
        rv = EBUSY;
        int priv = !(m->__flg & PTHREAD_PROCESS_SHARED);
        if (!a_cas(&m->__lock, 0, self->tid)) {
            rv = 0;
        } else if (!futex_pi(&m->__lock, priv, FUTEX_TRYLOCK_PI)) {
            if (m->__waiters == 2) {
                assert(m->__flg & PTHREAD_MUTEX_ROBUST);
                futex_pi(&m->__lock, priv, FUTEX_UNLOCK_PI);
                rv = ENOTRECOVERABLE;
            } else if ((m->__lock & FUTEX_OWNER_DIED) || m->__waiters == 1) {
                m->__waiters = 1;
                if (m->__flg & PTHREAD_MUTEX_ROBUST)
                    rv = EOWNERDEAD;
                else {
                    futex_pi(&m->__lock, priv, FUTEX_UNLOCK_PI);
                    rv = EBUSY;
                }
            }
        }
    } else {
        while (rv == -1) {
            int v = m->__lock;
            rv = EBUSY;
            if ((m->__flg & PTHREAD_MUTEX_ROBUST)
                    && (v & FUTEX_NR_TID_MASK) == FUTEX_NR_TID_MASK)
                rv = ENOTRECOVERABLE;
            else if (!(v & mask)) {
                rv = (m->__flg & PTHREAD_MUTEX_ROBUST) && (v & FUTEX_OWNER_DIED)? EOWNERDEAD : 0;
                if (a_cas(&m->__lock, v, v | self->tid) != v)
                    rv = -1;
            }
        }
    }

    if (rv == 0 || rv == EOWNERDEAD) {
        m->__next = self->robust.head;
        if (m->__next != (void *)&self->robust.head)
            m->__next->__prev = m;
        m->__prev = 0;
        self->robust.head = m;
    }
    __asm__("" ::: "memory");
    self->robust.pending = 0;
    return rv;
}
weak_alias(pthread_mutex_trylock, __pthread_mutex_trylock);
