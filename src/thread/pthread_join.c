#include <pthread.h>
#include <sys/mman.h>
#include <errno.h>
#include "futex.h"

int pthread_join(pthread_t t, void **pr)
{
    if (t == __pthread_self()) return EDEADLK;
    if (t->detachstate == DT_DETACHED) return EINVAL;
    pthread_testcancel();
    while (t->detachstate != DT_EXITING)
        __futex_wait_cp(&t->detachstate, 1, DT_JOINABLE);
    __tl_sync();
    if (pr) *pr = t->result;
    if (t->map)
        munmap(t->map, t->map_size);
    return 0;
}
