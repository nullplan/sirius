#include <fenv.h>
#include "fenv-ppc.h"

int fesetround(int rm)
{
    if (rm & ~FE_TOWARDZERO) return -1;
    __set_fpcr((__fpcr() & ~FE_TOWARDZERO) | rm);
    return 0;
}
