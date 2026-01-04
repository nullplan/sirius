#include "libm.h"
#pragma STDC FENV_ACCESS ON

/* ln 2 = 0.1011000101110010000101111111011111010001110011110111100110101011110010011110001110110011100110000000001111110010111101101010111101000000111100110100001100100110011100101001100010110110001011011000101000001101000101110101101110001011101010101111101000101..._2 */

static const double ln2hi = 0x1.62e42fefa39efP-1;
static const double ln2lo = 0x1.abc9f3b39803fP-56;

static const double C[7] = {
    2.0/3.0,
    2.0/5.0,
    2.0/7.0,
    2.0/9.0,
    2.0/11.0,
    2.0/13.0,
    2.0/15.0,
};

#ifdef __GNUC__
#define fabs(x) __builtin_fabs(x)
#endif

double log(double x)
{
    if (x == 0) return -1/fabs(x);
    if (signbit(x)) return (x-x)/(x-x);
    if (!isfinite(x)) return x;

    int k;
    x = frexp(x, &k);
    if (x <= M_SQRT1_2)
        x *= 2, k--;
    double f = x - 1.0;
    int ex = (__double_bits(f) >> 52) & 0x7ff;
    if (ex < 0x3ff - 20)
    {
        if (f == 0) return k * ln2hi + k * ln2lo;
        double R = f * f * (0.5 - (1.0/3.0) * f);
        return k * ln2hi + (f - (R - k * ln2lo));
    }

    double s = f/(2+f);
    double s2 = s*s;
    double R = C[6];
    for (int i = 6; i-- > 0;)
        R = R * s2 + C[i];
    /* actual R = (2 + R) * s */
    return k * ln2hi + (R * s + k * ln2lo + 2.0 * s);
}
