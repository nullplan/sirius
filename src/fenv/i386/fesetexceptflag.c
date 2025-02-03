#include <fenv.h>
#include <pthread.h>
#include "fenv-x86.h"

int fesetexceptflag(const fexcept_t *flag, int x)
{
    if (__pthread_self()->hwcap & HWCAP_SSE)
        __set_mxcsr(__mxcsr() | (flag->__mxcsr & x));
    else {
        uint16_t sw = __sw();
        if (~sw & flag->__mxcsr & x) {
            struct x87_fenv env = {
                .sw = sw | (flag->__mxcsr & x),
                .cw = __cw(),
                .tw = DEFAULT_X87_TW,
            };
            __set_env(&env);
        }
    }
    return 0;
}
