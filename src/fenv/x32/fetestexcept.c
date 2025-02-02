#include <fenv.h>
#include "fenv-x86.h"

int fetestexcept(int x)
{
    return (__mxcsr() | __sw()) & x & FE_ALL_EXCEPT;
}
