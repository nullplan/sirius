#include <fenv.h>
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

    __set_cw(cw);
    __set_mxcsr(mxcsr | (sw & FE_ALL_EXCEPT));
    __clex();
    return 0;
}
