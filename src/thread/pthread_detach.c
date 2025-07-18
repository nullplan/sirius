#include <pthread.h>
#include <errno.h>
#include <sys/mman.h>
#include "cpu.h"

hidden int __pthread_detach(pthread_t t)
{
    int v = a_cas(&t->detachstate, DT_JOINABLE, DT_DETACHED);
    if (v == DT_DETACHED) return EINVAL;
    if (v == DT_EXITING && t->map) {
        __tl_sync();
        __munmap(t->map, t->map_size);
    }
    return 0;
}
weak_alias(pthread_detach, __pthread_detach);
