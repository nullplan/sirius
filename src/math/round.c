#include "libm.h"
#include <fenv.h>
#pragma STDC FENV_ACCESS on

#ifdef __GNUC__
#define fabs(x) __builtin_fabs(x)
#endif

double round(double x)
{
    static const double toint = 1/DBL_EPSILON;
    if (isnan(x) || fabs(x) >= toint || x == 0.0) return x;
    int rm = fegetround();
    fesetround(FE_TONEAREST);
    if (signbit(x))
        x = x - toint + toint;
    else
        x = x + toint - toint;
    fesetround(rm);
    return x;
}
