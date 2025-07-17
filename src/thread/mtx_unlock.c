#include <threads.h>
#include <pthread.h>

int mtx_unlock(mtx_t *mtx)
{
    int rv = __pthread_mutex_unlock(mtx);
    return rv == 0? thrd_success : thrd_error;
}
