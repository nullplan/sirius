#include <fenv.h>
#include "fenv-aarch64.h"

int fesetenv(const fenv_t *fe)
{
    __set_fpsr(fe == FE_DFL_ENV? 0 : fe->__fpsr);
    __set_fpcr(fe == FE_DFL_ENV? 0 : fe->__fpcr);
    return 0;
}
