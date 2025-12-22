#include "libm.h"
#pragma STDC FENV_ACCESS ON

#ifdef __GNUC__
#define fabsl(x) __builtin_fabsl(x)
#define scalblnl(x, y) __builtin_scalblnl(x, y)
#define sqrtl(x) __builtin_sqrtl(x)
#endif

long double hypotl(long double x, long double y)
{
    #if LDBL_MANT_DIG==53
    return hypot(x, y);
    #else
    x = fabsl(x);
    y = fabsl(y);
    union ldshape ldx = {x}, ldy = {y};
    if (ldx.f < ldy.f) {
        ldx.f = y;
        ldy.f = x;
    }
    if (ldx.i.se - ldy.i.se > 128) return x + y;
    long double scale = 1;
    if (ldx.i.se >= (0x3fff+8192)) {
        if (ldx.i.se == 0x7fff) {
            if (isinf(x) || isinf(y)) return INFINITY;
            return NAN;
        }
        x *= 0x1p-9000L;
        y *= 0x1p-9000L;
        scale = 0x1p9000L;
    } else if (ldy.i.se < (0x3fff-8192)) {
        x *= 0x1p9000L;
        y *= 0x1p9000L;
        scale = 0x1p-9000L;
    }
    long double t = sqrtl(x*x+y*y);
    return t * scale;
    #endif
}
