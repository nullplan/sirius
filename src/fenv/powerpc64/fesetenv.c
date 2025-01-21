#include <fenv.h>
#include "fenv-ppc.h"

int fesetenv(const fenv_t *fe)
{
    __set_fpscr(fe == FE_DFL_ENV? 0 : fe->__fpscr);
    return 0;
}
