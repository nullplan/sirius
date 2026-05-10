#include "ldso.h"
#include <pthread.h>

static pthread_rwlock_t listlock = PTHREAD_RWLOCK_INITIALIZER;

hidden void __dl_wrlock(void)
{
    pthread_rwlock_wrlock(&listlock);
}

hidden void __dl_rdlock(void)
{
    pthread_rwlock_rdlock(&listlock);
}

hidden void __dl_unlock(void)
{
    pthread_rwlock_unlock(&listlock);
}
