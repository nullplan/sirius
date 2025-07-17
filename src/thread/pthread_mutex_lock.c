#include <pthread.h>
#include <time.h>

int __pthread_mutex_lock(pthread_mutex_t *m)
{
    return __pthread_mutex_clocklock(m, CLOCK_REALTIME, 0);
}
weak_alias(pthread_mutex_lock, __pthread_mutex_lock);
