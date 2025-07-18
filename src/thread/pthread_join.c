#include <pthread.h>
#include <sys/mman.h>
#include <errno.h>
#include "futex.h"

hidden int __pthread_join(pthread_t t, void **pr)
{
    if (t == __pthread_self()) return EDEADLK;
    if (t->detachstate == DT_DETACHED) return EINVAL;
    __pthread_testcancel();
    while (t->detachstate != DT_EXITING)
        __futex_wait_cp(&t->detachstate, 1, DT_JOINABLE);
    __tl_sync();
    if (pr) *pr = t->result;
    if (t->map)
        __munmap(t->map, t->map_size);
    return 0;
}
weak_alias(pthread_join, __pthread_join);
