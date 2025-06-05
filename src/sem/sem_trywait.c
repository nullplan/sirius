#include <semaphore.h>
#include <limits.h>
#include <errno.h>
#include "cpu.h"

int sem_trywait(sem_t *sem)
{
    int val = sem->__ctr;
    int n;
    for (;;)
    {
        if (!(val & INT_MAX)) {
            errno = EAGAIN;
            return -1;
        }
        n = a_cas(&sem->__ctr, val, val - 1);
        if (val == n) return 0;
        val = n;
    }
}
