#include <pthread.h>
#include "cpu.h"

int pthread_spin_unlock(pthread_spinlock_t *sl)
{
    a_swap(&sl->__lck, 0);
    return 0;
}
