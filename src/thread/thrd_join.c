#include <threads.h>
#include <pthread.h>

int thrd_join(thrd_t t, int *ir)
{
    void *pr;
    int val = __pthread_join(t, &pr);
    if (val) return thrd_error;
    if (ir) *ir = (uintptr_t)pr;
    return thrd_success;
}
