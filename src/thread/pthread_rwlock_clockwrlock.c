#include <pthread.h>
#include <errno.h>
#include "cpu.h"
#include "futex.h"

int pthread_rwlock_clockwrlock(pthread_rwlock_t *restrict rw, clockid_t clk, const struct timespec *restrict to)
{
    for (;;)
    {
        int val = rw->__lock;
        if (!val && !a_cas(&rw->__lock, 0, -1)) return 0;
        int rv = __timedwait(&rw->__lock, &rw->__waiters, val, to, clk);
        if (rv == -ETIMEDOUT) return ETIMEDOUT;
    }
}
