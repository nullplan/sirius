#include <fenv.h>
#include "fenv-x86.h"

int feraiseexcept(int x)
{
    __set_mxcsr(__mxcsr() | (x & FE_ALL_EXCEPT));
    return 0;
}
