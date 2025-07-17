#include <threads.h>
#include <pthread.h>

int thrd_detach(thrd_t t)
{
    return __pthread_detach(t)? thrd_error : thrd_success;
}
