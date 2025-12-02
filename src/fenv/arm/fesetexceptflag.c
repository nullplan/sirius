#include <fenv.h>
#include "fenv-arm.h"

int fesetexceptflag(const fexcept_t *flag, int x)
{
    #ifdef __ARM_PCS_VFP
    __set_fpscr(__fpscr() | (flag->__fpscr & x));
    #endif
    return 0;
}
