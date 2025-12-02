#include <fenv.h>
#include "fenv-arm.h"

int fegetenv(fenv_t *fe)
{
    #ifdef __ARM_PCS_VFP
    fe->__fpscr = __fpscr();
    #else
    fe->__fpscr = 0;
    #endif
    return 0;
}
