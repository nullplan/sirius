#include <pthread.h>
#include <errno.h>
#include "cpu.h"

int pthread_spin_lock(pthread_spinlock_t *sl)
{
    while (a_swap(&sl->__lck, EBUSY))
        a_pause();
    return 0;
}
