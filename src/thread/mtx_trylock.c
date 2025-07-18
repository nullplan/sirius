#include <threads.h>
#include <pthread.h>

int mtx_trylock(mtx_t *mtx)
{
    int rv = __pthread_mutex_trylock(mtx);
    return rv == 0? thrd_success : thrd_error;
}
