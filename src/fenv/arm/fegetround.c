#include <fenv.h>
#include "fenv-arm.h"

int fegetround(void)
{
    #ifdef __ARM_PCS_VFP
    return __fpscr() & FE_TOWARDZERO;
    #else
    return 0;
    #endif
}
