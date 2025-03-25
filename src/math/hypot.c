#include "libm.h"

#ifdef __GNUC__
#define fabs(x) __builtin_fabs(x)
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
    double scale = 1;
    /* if x >= 2^512, then x² overflows. But also, if x >= 2^511, y might also be
     * as big as 2^511, and then x²+y² overflows. */
    if (ix >= 0x5feULL << 52)
    {
        if (ix >= 0x7ffULL << 52) {
            if (ix == 0x7ffULL << 52 || iy == 0x7ffULL << 52) return INFINITY;
            return NAN;
        }
        x *= 0x1p-600;
        y *= 0x1p-600;
        scale = 0x1p600;
    } else if (iy < 0x1ffULL << 52) {
        /* if y < 2^-512, then y² underflows. */
        x *= 0x1p600;
        y *= 0x1p600;
        scale = 0x1p-600;
    }
    double t = sqrt(x*x+y*y);
    return t * scale;
}
