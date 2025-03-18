#include "libm.h"

static const double S1 = -1.0/6.0;
static const double S2 = +1.0/120.0;
static const double S3 = -1.0/5040.0;
static const double S4 = +1.0/362880.0;
static const double S5 = -1.0/39916800.0;
static const double S6 = +1.0/6227020800.0;

extern hidden double __kernel_sin(double x, double y, int iszero)
{
    int e = (__double_bits(x) >> 52) & 0x7ff;
    if (e < 0x3ff-27) return x;
    double x2 = x * x;
    double r = x * x2 * (S2 + x2 * (S3 + x2 * (S4 + x2 * (S5 + x2 * S6))));
    if (iszero) return x * (1 + x2 * (S1 + x2 * r));
    return x + (S1 * x * x2 + (x2 * (r - y/2) + y));
}
