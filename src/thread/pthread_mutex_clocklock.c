#include <pthread.h>
#include <errno.h>
#include <time.h>
#include "syscall.h"
#include "futex.h"
#include "libc.h"
#include "cpu.h"

static int do_futex_pi_lock(pthread_mutex_t *restrict m, clockid_t clk, const struct timespec *restrict ts)
{
    /* FUTEX_LOCK_PI takes the timeout as absolute CLOCK_REALTIME timeout.
     * So if clk==CLOCK_REALTIME, we are happy and can pass the timeout to the syscall almost directly (except for __pad).
     * If clk==CLOCK_MONOTONIC, we could use FUTEX_LOCK_PI2, but that would raise the kernel requirement.
     * And for everything else, I have to recalc anyway.
     */
}
int pthread_mutex_clocklock(pthread_mutex_t *restrict m, clockid_t clk, const struct timespec *restrict ts)
{
    /* I am not even entertaining the thought of putting CPU-time clocks here. Just get out with that! */
    if (clk == CLOCK_PROCESS_CPUTIME_ID || clk == CLOCK_THREAD_CPUTIME_ID || (clk < 0 && (clk & 3) != 3)) return EINVAL;

    int rv = pthread_mutex_trylock(m);
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

    int msk = m->__flg & PTHREAD_MUTEX_ROBUST? FUTEX_TID_MASK : FUTEX_NR_TID_MASK;
    int priv = !(m->__flg & PTHREAD_PROCESS_SHARED);
    while ((rv = pthread_mutex_trylock(m)) == EBUSY) {
        int v = m->__lock;
        if (!(v & msk)) continue; /* leave the complicated stuff to trylock */
        if (!(v & FUTEX_WAITERS)) {
            if (a_cas(&m->__lock, v, v | FUTEX_WAITERS) != v) continue;
            v |= FUTEX_WAITERS;
        }
        rv = -__timedwait(&m->__lock, &m->__waiters, v, priv, ts, clk);
        if (rv != 0 && rv != EAGAIN) break;
    }
    return rv;
}
