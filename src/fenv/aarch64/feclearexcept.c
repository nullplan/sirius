#include <fenv.h>
#include "fenv-aarch64.h"

int feclearexcept(int x)
{
    x &= FE_ALL_EXCEPT;
    __set_fpsr(__fpsr() & ~x);
    return 0;
}
