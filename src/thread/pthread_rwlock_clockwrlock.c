#include <pthread.h>
#include <errno.h>
#include "cpu.h"
#include "futex.h"

hidden int __pthread_rwlock_clockwrlock(pthread_rwlock_t *restrict rw, clockid_t clk, const struct timespec *restrict to)
{
    for (;;)
    {
        int val = rw->__lock;
        if (!val) {
            if (!a_cas(&rw->__lock, 0, -1)) return 0;
        } else {
            int rv = __timedwait(&rw->__lock, &rw->__waiters, val, !rw->__ps, to, clk);
            if (rv == -ETIMEDOUT) return ETIMEDOUT;
        }
    }
}
weak_alias(pthread_rwlock_clockwrlock, __pthread_rwlock_clockwrlock);
