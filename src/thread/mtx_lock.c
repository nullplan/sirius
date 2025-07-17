#include <threads.h>
#include <pthread.h>

int mtx_lock(mtx_t *mtx)
{
    return __pthread_mutex_lock(mtx) == 0? thrd_success : thrd_error;
}
