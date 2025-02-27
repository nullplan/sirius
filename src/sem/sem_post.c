#include <semaphore.h>
#include <errno.h>
#include <limits.h>
#include "cpu.h"

int sem_post(sem_t *sem)
{
    int waiters;
    int ctr;
    int priv = !sem->__pshared;
    for (;;) {
        ctr = sem->__ctr;
        if (ctr == INT_MAX) {
            errno = EOVERFLOW;
            return -1;
        }

        waiters = sem->__waiters;
        if (a_cas(&sem->__ctr, ctr, ctr + 1 + (ctr < 0)) == ctr)
            break;
    }
    if (waiters || ctr < 0)
        __futex_wake(&sem->__ctr, priv, 1);
    return 0;
}
