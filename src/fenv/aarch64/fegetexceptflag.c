#include <fenv.h>
#include "fenv-aarch64.h"

int fegetexceptflag(fexcept_t *flag, int x)
{
    x &= FE_ALL_EXCEPT;
    flag->__fpsr = __fpsr() & x;
    return 0;
}
