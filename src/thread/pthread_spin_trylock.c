#include <pthread.h>
#include <errno.h>
#include "cpu.h"

int pthread_spin_trylock(pthread_spinlock_t *sl)
{
    return a_swap(&sl->__lck, EBUSY);
}
