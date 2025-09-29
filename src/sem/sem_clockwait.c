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
            continue;
        }
        if (val == 0) {
            int n = a_cas(&sem->__ctr, 0, INT_MIN);
            if (n) {
                val = n;
                continue;
            }
            val = INT_MIN;
        }
        a_inc(&sem->__waiters);
        int rv = __timedwait(&sem->__ctr, val, !sem->__pshared, to, clk);
        a_dec(&sem->__waiters);
        if (rv == -EINTR || rv == -ETIMEDOUT) {
            errno = -rv;
            return -1;
        }
        val = sem->__ctr;
    }
}
