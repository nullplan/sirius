#include <fenv.h>
#include "fenv-ppc.h"

int fesetexceptflag(const fexcept_t *flag, int x)
{
    __set_fpscr(__fpscr() | (flag->__fpscr & x));
    return 0;
}
