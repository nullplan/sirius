#include "libm.h"

static const double othresh = 0x1.62e42fefa39efP9,      /* 1024 ln 2 */
             uthresh = -0x1.74385446d71c3P9,            /* -1074 ln 2 */
             ln2hi = 0x1.62e42fefa39efP-1,              /* ln 2 */
             ln2lo = 0x1.5b6f3e1f32e0aP-57,             /* ln 2 - ln2hi */
             P1   =  0x1.555555555553Ep-3,
             P2   = -0x1.6C16C16BEBD93p-9,
             P3   =  0x1.1566AAF25DE2Cp-14,
             P4   = -0x1.BBD41C5D26BF1p-20,
             P5   =  0x1.6376972BEA4D0p-25;

#ifdef __GNUC__
#define fabs(x) __builtin_fabs(x)
#endif

double exp(double x)
{
    if (!isfinite(x)) {
        if (isnan(x) || x > 0) return x;
        return 0;
    }
    int k;
    double hi, lo, r, z, R;
    if (x >= othresh) return scalbln(x, 1024);
    if (x < uthresh) return scalbln(-1/x, -1024);

    double absx = fabs(x);
    if (absx <= 0x1p-28) {
        FORCE_EVAL(0x1p54 + x); /* raise inexact if x != 0 */
        return 1 + x;
    }
    if (absx <= ln2hi/2) {
        r = x;
        k = 0;
    } else {
        if (absx <= 3.0*ln2hi/2) {
            hi = copysign(absx - ln2hi, x);
            lo = copysign(ln2lo, x);
            k = signbit(x)? -1 : 1;
        } else {
            k = absx/ln2hi + 0.5;
            if (signbit(x)) k = -k;
            hi = x - k * ln2hi;
            lo = k * ln2lo;
        }
        r = hi - lo;
    }

    z = r*r;
    /* Horner scheme */
    R = 2 + z * (P1 + z * (P2 + z * (P3 + z * (P4 + z * P5))));

    z = 1 + 2*r/(R - r);
    return scalbln(z, k);
}
