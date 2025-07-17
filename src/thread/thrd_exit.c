#include <threads.h>
#include <pthread.h>

_Noreturn void thrd_exit(int val)
{
    __pthread_exit((void *)(uintptr_t)val);
}
