#include <fenv.h>
#include "fenv-x86.h"

int feclearexcept(int x)
{
    x &= FE_ALL_EXCEPT;
    uint16_t sw = __sw();
    uint32_t mxcsr = __mxcsr();
    if (sw & FE_ALL_EXCEPT) {
        mxcsr |= sw & FE_ALL_EXCEPT;
        __clex();
    }
    __set_mxcsr(mxcsr & ~x);
    return 0;
}
