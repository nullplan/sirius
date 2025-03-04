#include <pthread.h>
#include <errno.h>
#include <limits.h>
#include "cpu.h"

int pthread_rwlock_tryrdlock(pthread_rwlock_t *rw)
{
    for (;;) {
        int val = rw->__lock;
        if (val == -1) return EBUSY;
        if (val == INT_MAX) return EAGAIN;
        if (a_cas(&rw->__lock, val, val + 1) == val) return 0;
    }
}
