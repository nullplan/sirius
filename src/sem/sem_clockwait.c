#include <semaphore.h>
#include <errno.h>
#include "futex.h"
#include "cpu.h"

int sem_clockwait(sem_t *restrict sem, clockid_t clk,
        const struct timespec *restrict to)
{
    int val = sem->__ctr;
    for (;;) {
        if (val > 0) {
            int n = a_cas(&sem->__ctr, val, val - 1);
            if (n == val) return 0;
            val = n;
        }
        if (val == 0 && (val = a_cas(&sem->__ctr, 0, -1))) continue;
        int rv = __timedwait(&sem->__ctr, &sem->__waiters, !sem->__pshared, to, clk);
        if (rv == -EINTR || rv == -ETIMEDOUT) {
            errno = -rv;
            return -1;
        }
    }
}
