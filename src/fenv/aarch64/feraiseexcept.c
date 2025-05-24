#include <fenv.h>
#include "fenv-aarch64.h"

int feraiseexcept(int x)
{
    x &= FE_ALL_EXCEPT;
    __set_fpsr(__fpsr() | x);
    return 0;
}
