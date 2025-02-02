#include <fenv.h>
#include "fenv-x86.h"

int fegetround(void)
{
    return __cw() & 0xc00;
}
