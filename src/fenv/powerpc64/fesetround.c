#include <fenv.h>
#include "fenv-ppc.h"

int fesetround(int rm)
{
    if (rm > 3u) return -1;
    __set_fpscr((__fpscr() & ~3) | rm);
    return 0;
}
