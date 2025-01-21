#include <fenv.h>
#include "fenv-ppc.h"

int feraiseexcept(int x)
{
    x &= FE_ALL_EXCEPT;
    if (x & FE_INVALID) x |= FE_INVALID_SOFTWARE;
    __set_fpscr(__fpscr() | x);
    return 0;
}
