#include <fenv.h>
#include "fenv-arm.h"

int fetestexcept(int x)
{
    #ifdef __ARM_PCS_VFP
    return __fpscr() & x & FE_ALL_EXCEPT;
    #else
    return 0;
    #endif
}
