#include <pthread.h>
#include <limits.h>
#include <errno.h>
#include "futex.h"
#include "cpu.h"

hidden int __pthread_rwlock_unlock(pthread_rwlock_t *rw)
{
    for (;;)
    {
        int val = rw->__lock;
        if (val == -1 && a_cas(&rw->__lock, -1, 0) == -1) {
            __futex_wake(&rw->__lock, 0, INT_MAX);
            return 0;
        } else if (val > 0 && a_cas(&rw->__lock, val, val - 1) == val) {
            if (val == 1)
                __futex_wake(&rw->__lock, 0, 1);
            return 0;
        } else if (val == 0)
            return EINVAL;
    }
}
weak_alias(pthread_rwlock_unlock, __pthread_rwlock_unlock);
