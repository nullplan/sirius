#include "libm.h"

#ifdef __GNUC__
#define fabs(x) __builtin_fabs(x)
#define scalbln(x,y) __builtin_scalbln(x,y)
#define sqrt(x) __builtin_sqrt(x)
#endif

double hypot(double x, double y)
{
    x = fabs(x);
    y = fabs(y);
    uint64_t ix = __double_bits(x);
    uint64_t iy = __double_bits(y);

    if (ix < iy) {
        ix = __double_bits(y);
        iy = __double_bits(x);
    }
    /* limits ratio to something sensible:
     * If x/y > 2^54, x+y is different from x only in
     * non-nearest rounding, and so is hypot(x,y).
     */
    if (ix - iy > 54ull << 52) return x + y;
    int scale = 0;
    /* if x >= 2^512, then x² overflows. */
    if (ix >= 0x5ffULL << 52)
    {
        if (ix >= 0x7ffULL << 52) {
            if (ix == 0x7ffULL << 52 || iy == 0x7ffULL << 52) return INFINITY;
            return NAN;
        }
        x = scalbln(x, -600);
        y = scalbln(y, -600);
        scale = 600;
    } else if (iy < 0x1ffULL << 52) {
        /* if y < 2^-512, then y² underflows. */
        x = scalbln(x, 600);
        y = scalbln(y, 600);
        scale = -600;
    }
    double t = sqrt(x*x+y*y);
    if (scale) t = scalbln(t, scale);
    return t;
}
