#include <pthread.h>
#include <errno.h>
#include "futex.h"
#include "cpu.h"

int pthread_mutex_consistent(pthread_mutex_t *m)
{
    int v = m->__lock;
    if (!(m->__flg & PTHREAD_MUTEX_ROBUST))
        return EINVAL;

    if (m->__flg & PTHREAD_PRIO_INHERIT) {
        if (m->__waiters != 1) return EINVAL;
        m->__waiters = 0;
        return 0;
    }
    for (;;) {
        int n = a_cas(&m->__lock, v, v & ~FUTEX_OWNER_DIED);
        if (n == v) return 0;
        v = n;
    }
}
