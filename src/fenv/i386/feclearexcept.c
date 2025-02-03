#include <fenv.h>
#include <pthread.h>
#include "fenv-x86.h"

int feclearexcept(int x)
{
    x &= FE_ALL_EXCEPT;
    uint16_t sw = __sw();
    if (__pthread_self()->hwcap & HWCAP_SSE) {
        uint32_t mxcsr = __mxcsr();
        if (sw & FE_ALL_EXCEPT) {
            mxcsr |= sw & FE_ALL_EXCEPT;
            __clex();
        }
        __set_mxcsr(mxcsr & ~x);
    } else {
        sw &= ~x;
        if (!(sw & FE_ALL_EXCEPT))
            __clex();
        else {
           struct x87_fenv env = {
               .sw = sw,
               .cw = __cw(),
               .tw = DEFAULT_X87_TW,
           };
           __set_env(&env);
        }
    }
    return 0;
}
