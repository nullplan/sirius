#include <fenv.h>
#include "fenv-ppc.h"

int fegetround(void)
{
    return __fpsr() & FE_TOWARDZERO;
}
