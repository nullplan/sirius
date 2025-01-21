#include <fenv.h>
#include "fenv-ppc.h"

int fegetround(void)
{
    #ifdef __NO_FPRS__
    return FE_TONEAREST;
    #else
    return __fpscr() & 3;
    #endif
}
