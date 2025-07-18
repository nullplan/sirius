#include <threads.h>
#include <pthread.h>
#include <errno.h>
#include "libc.h"

int thrd_create(thrd_t *t, thrd_start_t f, void *arg)
{
    pthread_attr_t a = {0};
    a.__ss = __default_stacksize;
    a.__gs = DEFAULT_GUARD_SIZE;
    a.__flags = __PTHREAD_ATTR_C11;
    int rv = __pthread_create(t, &a, (void *(*)(void *))f, arg);
    return rv == EAGAIN? thrd_nomem : rv? thrd_error : thrd_success;
}
