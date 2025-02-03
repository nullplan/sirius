#include <fenv.h>
#include "fenv-ppc.h"

int feraiseexcept(int x)
{
    x &= FE_ALL_EXCEPT;
    __set_fpsr(__fpsr() | x);
    return 0;
}
