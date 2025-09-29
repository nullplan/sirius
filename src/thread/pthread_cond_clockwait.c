#include <pthread.h>
#include <errno.h>
#include "futex.h"
#include "cpu.h"

static void cleanup(void *m)
{
    __pthread_mutex_lock(m);
}

static void decw(void *c)
{
    a_dec(&((pthread_cond_t *)c)->__waiters);
}

hidden int __pthread_cond_clockwait(pthread_cond_t *restrict c, pthread_mutex_t *restrict m, clockid_t clk, const struct timespec *restrict to)
{
    int serial = c->__serial;
    int rv = __pthread_mutex_unlock(m);
    if (rv) return rv;

    pthread_cleanup_push(cleanup, m);
    while ((rv == 0 || rv == EINTR) && c->__serial == serial)
    {
        pthread_cleanup_push(decw, c);
        a_inc(&c->__waiters);
        rv = -__timedwait_cp(&c->__serial, serial, !c->__ps, to, clk);
        pthread_cleanup_pop(1);
    }
    pthread_cleanup_pop(0);

    int lockrv = __pthread_mutex_lock(m);
    if (rv != ETIMEDOUT && rv != EINVAL) rv = lockrv;
    return rv;
}
weak_alias(pthread_cond_clockwait, __pthread_cond_clockwait);
