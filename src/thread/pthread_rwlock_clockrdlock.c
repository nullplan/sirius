#include <pthread.h>
#include <errno.h>
#include <limits.h>
#include "cpu.h"
#include "futex.h"

int pthread_rwlock_clockrdlock(pthread_rwlock_t *restrict rw, clockid_t clk, const struct timespec *restrict to)
{
    for (;;)
    {
        int val = rw->__lock;
        if (val == INT_MAX) return EAGAIN;
        if (val >= 0 && a_cas(&rw->__lock, val, val + 1) == val) return 0;
        int rv = __timedwait(&rw->__lock, &rw->__waiters, val, to, clk);
        if (rv == -ETIMEDOUT) return ETIMEDOUT;
    }
}
