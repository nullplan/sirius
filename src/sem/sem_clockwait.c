#include <semaphore.h>
#include <limits.h>
#include <errno.h>
#include "futex.h"
#include "cpu.h"

int sem_clockwait(sem_t *restrict sem, clockid_t clk,
        const struct timespec *restrict to)
{
    int val = sem->__ctr;
    for (;;) {
        if (val & INT_MAX) {
            int n = (val & INT_MAX) - 1;
            if (sem->__waiters) n += INT_MIN;
            n = a_cas(&sem->__ctr, val, n);
            if (n == val) return 0;
            val = n;
        }
        if (val >= 0) {
            int n = a_cas(&sem->__ctr, val, INT_MIN + val);
            if (n != val) {
                val = n;
                continue;
            }
            val += INT_MIN;
        }
        int rv = __timedwait(&sem->__ctr, &sem->__waiters, val, !sem->__pshared, to, clk);
        if (rv == -EINTR || rv == -ETIMEDOUT) {
            errno = -rv;
            return -1;
        }
    }
}
