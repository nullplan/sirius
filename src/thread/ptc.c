#include <pthread.h>
static pthread_rwlock_t ptc_lock = PTHREAD_RWLOCK_INITIALIZER;
hidden void __inhibit_ptc(void)
{
    pthread_rwlock_wrlock(&ptc_lock);
}

hidden void __acquire_ptc(void)
{
    pthread_rwlock_rdlock(&ptc_lock);
}

hidden void __release_ptc(void)
{
    pthread_rwlock_unlock(&ptc_lock);
}

