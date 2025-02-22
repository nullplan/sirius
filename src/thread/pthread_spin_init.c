#include <pthread.h>

int pthread_spin_init(pthread_spinlock_t *sl, int x)
{
    sl->__lck = 0;
    return 0;
}
