#include <fenv.h>
#include "fenv-ppc.h"

int fegetexceptflag(fexcept_t *flag, int x)
{
    x &= FE_ALL_EXCEPT;
    flag->__fpsr = __fpsr() & x;
    return 0;
}
