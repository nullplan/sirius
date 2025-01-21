#include <fenv.h>
#include "fenv-ppc.h"

int fetestexcept(int x)
{
    #ifdef __NO_FPRS__
    return 0;
    #else
    return __fpscr() & x & (FE_ALL_EXCEPT | FE_ALL_INVALID);
    #endif
}
