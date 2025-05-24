#include <fenv.h>
#include "fenv-aarch64.h"

int fegetround(void)
{
    return __fpsr() & FE_TOWARDZERO;
}
