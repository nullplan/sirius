#include <fenv.h>
#include "fenv-ppc.h"

int fesetexceptflag(const fexcept_t *flag, int x)
{
    __set_fpsr(__fpsr() | (flag->__fpsr & x));
    return 0;
}
