#include <pthread.h>
#include <errno.h>
#include <limits.h>
#include "cpu.h"
#include "futex.h"

hidden int __pthread_rwlock_clockrdlock(pthread_rwlock_t *restrict rw, clockid_t clk, const struct timespec *restrict to)
{
    for (;;)
    {
        int val = rw->__lock;
        if (val == INT_MAX) return EAGAIN;
        if (val >= 0) {
            if (a_cas(&rw->__lock, val, val + 1) == val) return 0;
        } else {
            a_inc(&rw->__waiters);
            int rv = __timedwait(&rw->__lock, val, !rw->__ps, to, clk);
            a_dec(&rw->__waiters);
            if (rv == -ETIMEDOUT) return ETIMEDOUT;
        }
    }
}
weak_alias(pthread_rwlock_clockrdlock, __pthread_rwlock_clockrdlock);
