#include <threads.h>
#include <pthread.h>
int cnd_wait(cnd_t *cnd, mtx_t *mtx)
{
    return cnd_timedwait(cnd, mtx, 0);
}
