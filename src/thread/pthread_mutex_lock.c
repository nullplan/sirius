#include <pthread.h>
#include <time.h>

int pthread_mutex_lock(pthread_mutex_t *m)
{
    return pthread_mutex_clocklock(m, CLOCK_REALTIME, 0);
}
