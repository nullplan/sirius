#include <fenv.h>
#include "fenv-arm.h"

int feraiseexcept(int x)
{
    #ifdef __ARM_PCS_VFP
    x &= FE_ALL_EXCEPT;
    __set_fpscr(__fpscr() | x);
    #endif
    return 0;
}
