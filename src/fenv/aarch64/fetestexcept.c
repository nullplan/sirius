#include <fenv.h>
#include "fenv-aarch64.h"

int fetestexcept(int x)
{
    return __fpsr() & x & FE_ALL_EXCEPT;
}
