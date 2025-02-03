#include <fenv.h>
#include <pthread.h>
#include "fenv-x86.h"

int feraiseexcept(int x)
{
    if (__pthread_self()->hwcap & HWCAP_SSE)
    {
        __set_mxcsr(__mxcsr() | (x & FE_ALL_EXCEPT));
    } else {
        uint16_t sw = __sw();
        x &= FE_ALL_EXCEPT;
        if (~sw & x) {
            struct x87_fenv env = {
                .cw = __cw(),
                .sw = sw | x,
                .tw = DEFAULT_X87_TW,
            };
            __set_env(&env);
        }
    }
    return 0;
}
