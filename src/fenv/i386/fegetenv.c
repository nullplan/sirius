#include <fenv.h>
#include <pthread.h>
#include "fenv-x86.h"

int fegetenv(fenv_t *fe)
{
    fe->__sw = __sw();
    fe->__cw = __cw();
    if (__pthread_self()->hwcap & HWCAP_SSE)
        fe->__mxcsr = __mxcsr();
    return 0;
}
