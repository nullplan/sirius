#include <fenv.h>
#include "fenv-arm.h"

int fesetenv(const fenv_t *fe)
{
    #ifdef __ARM_PCS_VFP
    __set_fpscr(fe == FE_DFL_ENV? 0 : fe->__fpscr);
    #endif
    return 0;
}
