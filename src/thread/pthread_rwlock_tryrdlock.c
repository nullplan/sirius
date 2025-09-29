#include <pthread.h>
#include <errno.h>
#include <limits.h>
#include "cpu.h"

int pthread_rwlock_tryrdlock(pthread_rwlock_t *rw)
{
    int val = rw->__lock;
    for (;;) {
        if (val == -1) return EBUSY;
        if (val == INT_MAX) return EAGAIN;
        int n = a_cas(&rw->__lock, val, val + 1);
        if (n == val) return 0;
        val = n;
    }
}
