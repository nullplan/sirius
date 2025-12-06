#include <fenv.h>
#include "fenv-arm.h"

int fesetround(int rm)
{
    #ifdef __ARM_PCS_VFP
    if (rm & ~FE_TOWARDZERO) return -1;
    __set_fpscr((__fpscr() & ~FE_TOWARDZERO) | rm);
    #else
    if (rm) return -1;
    #endif
    return 0;
}
