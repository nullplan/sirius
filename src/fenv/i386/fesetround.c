#include <fenv.h>
#include <pthread.h>
#include "fenv-x86.h"

int fesetround(int rm)
{
    if (rm & ~0xc00) return -1;
    __set_cw((__cw() & ~0xc00) | rm);
    if (__pthread_self()->hwcap & HWCAP_SSE)
        __set_mxcsr((__mxcsr() & ~0x6000) | (rm << 3));
    return 0;
}
