#include <pthread.h>

static void dummy(void) {}
weak_alias(__testcancel, dummy);
hidden void __pthread_testcancel(void)
{
    __testcancel();
}
weak_alias(pthread_testcancel, __pthread_testcancel);
