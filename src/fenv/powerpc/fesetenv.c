#include <fenv.h>
#include "fenv-ppc.h"

int fesetenv(const fenv_t *fe)
{
    #ifndef __NO_FPRS__
    __set_fpscr(fe == FE_DFL_ENV? 0 : fe->__fpscr);
    #endif
    return 0;
}
