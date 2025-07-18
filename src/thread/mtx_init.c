#include <threads.h>
#include <pthread.h>

int mtx_init(mtx_t *mtx, int type)
{
    pthread_mutexattr_t ma = {0};
    if (type & mtx_recursive)
        ma.__flg |= PTHREAD_MUTEX_RECURSIVE;
    return __pthread_mutex_init(mtx, &ma) == 0? thrd_success : thrd_error;
}
