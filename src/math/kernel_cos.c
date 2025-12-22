#include "libm.h"
#pragma STDC FENV_ACCESS ON

static const double C2 = +1.0/24.0;
static const double C3 = -1.0/720.0;
static const double C4 = +1.0/40320.0;
static const double C5 = -1.0/3628800.0;
static const double C6 = +1.0/479001600.0;
static const double C7 = -1.0/87178291200.0;

double __kernel_cos(double x, double y)
{
    int e = (__double_bits(x) >> 52) & 0x7ff;
    if (e < 0x3ff-27) {
        FORCE_EVAL(0x1p54 + x); /* raise inexact if x != 0 */
        return 1.0;
    }
    double x2 = x * x;
    double r = x2 * x2 * (C2 + x2 * (C3 + x2 * (C4 + x2 * (C5 + x2 * (C6 + x2 * C7)))));
    return 1 - (0.5 * x2 - (r - x * y));
}
