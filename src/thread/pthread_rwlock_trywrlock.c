#include <pthread.h>
#include <errno.h>
#include "cpu.h"

int pthread_rwlock_trywrlock(pthread_rwlock_t *rw)
{
    for (;;) {
        int val = rw->__lock;
        if (val) return EBUSY;
        if (!a_cas(&rw->__lock, 0, -1)) return 0;
    }
}
