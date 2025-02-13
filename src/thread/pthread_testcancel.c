#include <pthread.h>

static void dummy(void) {}
weak_alias(__testcancel, dummy);
void pthread_testcancel(void)
{
    __testcancel();
}
