#include "libm.h"
#pragma STDC FENV_ACCESS ON

/* ln 2 = 0.10110001011100100001011111110111110100011100111101111001101010111100100111100011101100111001100000000011111100101 11101101010111101000000111100110100001100100110011100101001100010110110001011011000101000001101000101110101101110001011101010101111101000101..._2 */

#if LDBL_MANT_DIG==64
static const long double ln2hi = 0x1.62e42fefa39ef356P-1L;
static const long double ln2lo = 0x1.93c7673007e5ed5eP-65L;
#elif LDBL_MANT_DIG==113
static const long double ln2hi = 0x1.62e42fefa39ef35793c7673007e5P-1L;
static const long double ln2lo = 0x1.dabd03cd0c99ca62d8b628345d6eP-114L;
#endif

long double logl(long double x)
{
    #if LDBL_MANT_DIG==53
    return log(x);
    #else
    static const long double C[10] = {
        2.0L/3.0L,
        2.0L/5.0L,
        2.0L/7.0L,
        2.0L/9.0L,
        2.0L/11.0L,
        2.0L/13.0L,
        2.0L/15.0L,
        2.0L/17.0L,
        2.0L/19.0L,
        2.0L/21.0L,
    };

    if (x == 0) return -DBL_MAX * DBL_MAX;
    if (signbit(x)) return (x-x)/(x-x);
    if (!isfinite(x)) return x;

    int k;
    x = frexpl(x, &k);
    if (x <= M_SQRT1_2l)
        x *= 2, k--;
    long double f = x - 1.0;
    union ldshape ldf = {f};
    int ex = ldf.i.se & 0x7fff;
    if (ex < 0x3fff - 20)
    {
        if (f == 0) return k * ln2hi + k * ln2lo;
        long double R = f * f * (0.5 - (1.0L/3.0L) * f);
        return k * ln2hi + (f - (R - k * ln2lo));
    }

    long double s = f/(2+f);
    long double s2 = s * s;
    long double R = C[9];
    for (int i = 9; i-- > 0;)
        R = R * s2 + C[i];
    return k * ln2hi + (R * s + k * ln2lo + 2.0 * s);
    #endif
}
