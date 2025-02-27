#include <semaphore.h>

int sem_getvalue(sem_t *restrict sem, int *restrict pval)
{
    int val = sem->__ctr;
    if (val < 0) val = 0;
    *pval = val;
    return 0;
}
