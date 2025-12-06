#include <fenv.h>
#include "fenv-arm.h"

int fegetexceptflag(fexcept_t *flag, int x)
{
    #ifdef __ARM_PCS_VFP
    x &= FE_ALL_EXCEPT;
    flag->__fpscr = __fpscr() & x;
    #else
    flag->__fpscr = 0;
    #endif
    return 0;
}
