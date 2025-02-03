#include <fenv.h>
#include "fenv-ppc.h"

int fetestexcept(int x)
{
    return __fpsr() & x & (FE_ALL_EXCEPT | FE_ALL_INVALID);
}
