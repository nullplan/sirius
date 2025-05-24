#include <pthread.h>

int pthread_condattr_getclock(const pthread_condattr_t *restrict ca, clockid_t *restrict clk)
{
    *clk = ca->__clk;
    return 0;
}

int pthread_condattr_getpshared(const pthread_condattr_t *restrict ca, int *restrict v)
{
    *v = ca->__flg;
    return 0;
}
