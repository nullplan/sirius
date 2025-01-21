#include <fenv.h>
#include "fenv-ppc.h"

int fegetexceptflag(fexcept_t *flag, int x)
{
    x &= FE_ALL_EXCEPT;
    if (x & FE_INVALID) x |= FE_ALL_INVALID;
    flag->__fpscr = __fpscr() & x;
    return 0;
}
