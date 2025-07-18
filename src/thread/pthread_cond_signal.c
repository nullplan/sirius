#include <pthread.h>
#include <threads.h>
#include <limits.h>
#include "futex.h"
#include "cpu.h"

static void cond_wait_core(pthread_cond_t *c, int val)
{
    a_inc(&c->__serial);
    __futex_wake(&c->__serial, !c->__ps, val);
}

static int __pthread_cond_broadcast(pthread_cond_t *c)
{
    cond_wait_core(c, INT_MAX);
    return 0;
}
weak_alias(pthread_cond_broadcast, __pthread_cond_broadcast);
weak_alias(cnd_broadcast, __pthread_cond_broadcast);

static int __pthread_cond_signal(pthread_cond_t *c)
{
    cond_wait_core(c, 1);
    return 0;
}
weak_alias(pthread_cond_signal, __pthread_cond_signal);
weak_alias(cnd_signal, __pthread_cond_signal);
