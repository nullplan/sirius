#include <fenv.h>
#include "fenv-x86.h"

int fegetexceptflag(fexcept_t *flag, int x)
{
    flag->__mxcsr = (__mxcsr() | __sw()) & x & FE_ALL_EXCEPT;
    return 0;
}
