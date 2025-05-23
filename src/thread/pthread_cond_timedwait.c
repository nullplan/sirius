#include <pthread.h>

int pthread_cond_timedwait(pthread_cond_t *restrict c, pthread_mutex_t *restrict m, const struct timespec *restrict to)
{
    return pthread_cond_clockwait(c, m, to, c->__clk);
}
