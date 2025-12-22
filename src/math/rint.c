#include "libm.h"
#pragma STDC FENV_ACCESS ON

double rint(double x)
{
    uint64_t ix = __double_bits(x);
    int exp = ((ix >> 52) & 0x7ff) - 0x3ff;
    if (exp >= 52)
        return x;

    double y;
    if (ix >> 63)
        y = x - 0x1p52 + 0x1p52;
    else
        y = x + 0x1p52 - 0x1p52;

    if (y == 0) return ix >> 63? -0.0 : +0.0;
    return y;
}
