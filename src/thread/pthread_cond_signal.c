#include <pthread.h>
#include <limits.h>
#include "futex.h"
#include "cpu.h"

static void cond_wait_core(pthread_cond_t *c, int val)
{
    a_inc(&c->__serial);
    __futex_wake(&c->__serial, !c->__ps, val);
}

int pthread_cond_broadcast(pthread_cond_t *c)
{
    cond_wait_core(c, INT_MAX);
    return 0;
}

int pthread_cond_signal(pthread_cond_t *c)
{
    cond_wait_core(c, 1);
    return 0;
}
