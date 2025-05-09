#include <semaphore.h>
#include <limits.h>
#include <errno.h>
#include "cpu.h"

int sem_trywait(sem_t *sem)
{
    int val = sem->__ctr;
    if ((val & INT_MAX) && a_cas(&sem->__ctr, val, val - 1) == val)
        return 0;
    errno = EAGAIN;
    return -1;
}
