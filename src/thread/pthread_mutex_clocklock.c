#include <pthread.h>
#include <errno.h>
#include <time.h>
#include <assert.h>
#include "syscall.h"
#include "futex.h"
#include "libc.h"
#include "cpu.h"

static int futex_pi(volatile int *fut, int priv, int op, const struct timespec *restrict ts)
{
    if (priv) priv = FUTEX_PRIVATE_FLAG;
    int rv;
    do {
        rv = __syscall(SYS_futex, fut, op | priv, 0, ts);
        if (rv == -ENOSYS && priv)
            rv = __syscall(SYS_futex, fut, op, 0, ts);
    } while (rv == -EINTR || rv == -EAGAIN);
    return rv;
}

static int do_futex_pi_lock(pthread_mutex_t *restrict m, clockid_t clk, const struct timespec *restrict ts)
{
    /* respect the minimum system requirements.
     * For clk==CLOCK_REALTIME, I can use FUTEX_LOCK_PI, and for clk==CLOCK_MONOTONIC, I have to use FUTEX_LOCK_PI2.
     * That does raise the kernel requirement for that case. Well, such is life.
     */
    if (!a_cas(&m->__lock, 0, __pthread_self()->tid)) return 0;

    int cmd = clk == CLOCK_REALTIME? FUTEX_LOCK_PI : FUTEX_LOCK_PI2;
    int priv = !(m->__flg & PTHREAD_PROCESS_SHARED);

    #ifndef __SIXTY_FOUR
    /* clear __pad */
    struct timespec lts;
    if (ts) {
        lts = (struct timespec){.tv_sec = ts->tv_sec, .tv_nsec = ts->tv_nsec};
        ts = &lts;
    }
    #endif

    int rv = futex_pi(&m->__lock, priv, cmd, ts);
    if (rv) return -rv;

    if (m->__waiters == 2) {
        assert(m->__flg & PTHREAD_MUTEX_ROBUST);
        futex_pi(&m->__lock, priv, FUTEX_UNLOCK_PI, 0);
        rv = ENOTRECOVERABLE;
    } else if ((m->__lock & FUTEX_OWNER_DIED) || m->__waiters == 1) {
        m->__waiters = 1;
        if (m->__flg & PTHREAD_MUTEX_ROBUST)
            rv = EOWNERDEAD;
        else {
            futex_pi(&m->__lock, priv, FUTEX_UNLOCK_PI, 0);
            do rv = -__timedwait(&(int){0}, &(int){0}, 0, 1, ts, clk);
            while (rv == EINTR);
        }
    }
    return rv;
}

hidden int __pthread_mutex_clocklock(pthread_mutex_t *restrict m, clockid_t clk, const struct timespec *restrict ts)
{
    if (clk != CLOCK_REALTIME && clk != CLOCK_MONOTONIC) return EINVAL;

    int rv = __pthread_mutex_trylock(m);
    if (rv != EBUSY) return rv;

    if (m->__flg & PTHREAD_PRIO_INHERIT) {
        pthread_t self = __pthread_self();
        self->robust.pending = m;
        rv = do_futex_pi_lock(m, clk, ts);
        if (rv == 0 || rv == EOWNERDEAD) {
            m->__next = self->robust.head;
            self->robust.head = m;
            if (m->__next != (void *)&self->robust.head)
                m->__next->__prev = m;
        }
        __asm__("" ::: "memory");
        self->robust.pending = 0;
        return rv;
    }

    int mask = m->__flg & PTHREAD_MUTEX_ROBUST? FUTEX_TID_MASK : FUTEX_NR_TID_MASK;
    int priv = !(m->__flg & PTHREAD_PROCESS_SHARED);
    while ((rv = pthread_mutex_trylock(m)) == EBUSY) {
        int v = m->__lock;
        if (!(v & mask)) continue; /* leave the complicated stuff to trylock */
        if (!(v & FUTEX_WAITERS)) {
            if (a_cas(&m->__lock, v, v | FUTEX_WAITERS) != v) continue;
            v |= FUTEX_WAITERS;
        }
        rv = -__timedwait(&m->__lock, &m->__waiters, v, priv, ts, clk);
        if (rv != 0 && rv != EAGAIN) break;
    }
    return rv;
}
weak_alias(pthread_mutex_clocklock, __pthread_mutex_clocklock);
