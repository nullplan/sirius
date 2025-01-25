#include <fenv.h>
#include "fenv-ppc.h"

int feclearexcept(int x)
{
    x &= FE_ALL_EXCEPT;
    if (x & FE_INVALID) x |= FE_ALL_INVALID;
    __set_fpscr(__fpscr() & ~x);
    return 0;
}
