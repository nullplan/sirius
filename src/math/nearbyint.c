#include "libm.h"
#include <fenv.h>
#pragma STDC FENV_ACCESS ON

/* If I supported interrupting environments,
 * this would be a great time to use feholdexcept().
 * But since I don't, this is even easier.
 */
double nearbyint(double x)
{
#ifndef FE_INEXACT
    return rint(x);
#else
    int p = fetestexcept(FE_INEXACT);
    x = rint(x);
    if (!p) feclearexcept(FE_INEXACT);
    return x;
#endif
}
