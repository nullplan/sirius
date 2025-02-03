#include <fenv.h>
#include <pthread.h>
#include "fenv-x86.h"

int fesetenv(const fenv_t *fe)
{
    uint16_t sw, cw;
    uint32_t mxcsr;
    if (fe == FE_DFL_ENV) {
        sw = 0;
        cw = DEFAULT_X87_CW;
        mxcsr = DEFAULT_MXCSR;
    } else {
        sw = fe->__sw;
        cw = fe->__cw;
        mxcsr = fe->__mxcsr;
    }

    if (__pthread_self()->hwcap & HWCAP_SSE) {
        __set_cw(cw);
        __clex();
        __set_mxcsr(mxcsr | (sw & FE_ALL_EXCEPT));
    } else {
        struct x87_fenv env = {
            .sw = sw,
            .cw = cw,
            .tw = DEFAULT_X87_TW,
        };
        __set_env(&env);
    }
    return 0;
}
