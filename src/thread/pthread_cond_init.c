#include <pthread.h>

int pthread_cond_init(pthread_cond_t *restrict c, const pthread_condattr_t *restrict ca)
{
    *c = (pthread_cond_t){0};
    if (ca) {
        c->__clk = ca->__clk;
        c->__ps = ca->__flg;
    }
    return 0;
}
