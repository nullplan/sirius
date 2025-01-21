#include <fenv.h>
#include "fenv-ppc.h"

int fesetexceptflag(const fexcept_t *flag, int x)
{
#ifndef __NO_FPRS__
    __set_fpscr(__fpscr() | (flag->__fpscr & x));
#endif
    return 0;
}
