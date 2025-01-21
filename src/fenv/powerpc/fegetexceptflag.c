#include <fenv.h>
#include "fenv-ppc.h"

int fegetexceptflag(fexcept_t *flag, int x)
{
#ifdef __NO_FPRS__
    flag->__fpscr = 0;
#else
    x &= FE_ALL_EXCEPT;
    if (x & FE_INVALID) x |= FE_ALL_INVALID;
    flag->__fpscr = __fpscr() & x;
#endif
    return 0;
}
