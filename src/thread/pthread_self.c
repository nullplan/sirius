#include <pthread.h>
#include <threads.h>

static pthread_t _pthread_self(void)
{
    return __pthread_self();
}
weak_alias(pthread_self, _pthread_self);
weak_alias(thrd_current, _pthread_self);
