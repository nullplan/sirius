#include <fenv.h>

int fetestexceptflag(const fexcept_t *flag, int x)
{
    return flag->__fpscr & x;
}
