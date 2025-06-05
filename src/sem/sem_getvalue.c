#include <semaphore.h>
#include <limits.h>

int sem_getvalue(sem_t *restrict sem, int *restrict pval)
{
    int val = sem->__ctr;
    *pval = val & INT_MAX;
    return 0;
}
