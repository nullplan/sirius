#include <fenv.h>
#include "fenv-x86.h"

int fesetexceptflag(const fexcept_t *flag, int x)
{
    __set_mxcsr(__mxcsr() | (flag->__mxcsr & x));
    return 0;
}
