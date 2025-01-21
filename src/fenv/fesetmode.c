#include <fenv.h>

int fesetmode(const femode_t *m)
{
    if (m == FE_DFL_MODE)
        return fesetround(FE_TONEAREST);
    return fesetround(m->__rm);
}
