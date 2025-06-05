#include <semaphore.h>
#include <errno.h>
#include <limits.h>
#include "cpu.h"

int sem_post(sem_t *sem)
{
    int ctr = sem->__ctr;
    int n; 
    int priv = !sem->__pshared;
    for (;;) {
        if ((ctr & INT_MAX) == INT_MAX) {
            errno = EOVERFLOW;
            return -1;
        }

        n = a_cas(&sem->__ctr, ctr, ctr + 1);
        if (n == ctr)
            break;
        ctr = n;
    }
    if (ctr < 0)
        __futex_wake(&sem->__ctr, priv, 1);
    return 0;
}
