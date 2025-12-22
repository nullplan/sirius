#include "libm.h"
#pragma STDC FENV_ACCESS ON

static const float othresh =  0x1.62e430P6,     /*  128 ln 2 */
                   uthresh = -0x1.9d1da0P6,     /* -149 ln 2 */
                   ln2hi =    0x1.62e42eP-1,    /*  ln 2 */
                   ln2lo =    0x1.efa39fP-25,   /*  ln 2 - ln2hi */
                   P1    =    0x1.555556P-3,
                   P2    =   -0x1.6c16c2P-9,
                   P3    =    0x1.1566aaP-14,
                   P4    =   -0x1.bbd41cP-20;

#ifdef __GNUC__
#define fabsf(x) __builtin_fabsf(x)
#endif

float expf(float x)
{
    if (!isfinite(x)) {
        if (isnan(x) || x > 0) return x;
        return 0;
    }
    int k;
    float hi, lo, r, z, R;

    if (x >= othresh) return scalblnf(x, 128);
    if (x < uthresh) return scalblnf(-1/x, -128);

    float absx = fabsf(x);
    if (absx <= 0x1p-13) {
        FORCE_EVAL(0x1p54f + x);
        return 1 + x;
    }
    if (absx <= ln2hi/2) {
        r = x;
        k = 0;
    } else {
        if (absx <= 3.0f * ln2hi/2) {
            hi = copysignf(absx - ln2hi, x);
            lo = copysignf(ln2lo, x);
            k = signbit(x)? -1 : 1;
        } else {
            k = absx/ln2hi + 0.5f;
            if (signbit(x)) k = -k;
            hi = x - k * ln2hi;
            lo = k * ln2lo;
        }
        r = hi - lo;
    }
     z = r*r;
     R = 2 + z * (P1 + z * (P2 + z * (P3 + z * P4)));
     z = 1 + 2*r/(R - r);
     return scalblnf(z, k);
}
