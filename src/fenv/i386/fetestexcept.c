#include <fenv.h>
#include <pthread.h>
#include "fenv-x86.h"

int fetestexcept(int x)
{
    uint32_t mxcsr = 0;
    if (__pthread_self()->hwcap & HWCAP_SSE)
        mxcsr = __mxcsr();
    return (mxcsr | __sw()) & x & FE_ALL_EXCEPT;
}
