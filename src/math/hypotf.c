#include "libm.h"

#ifdef __GNUC__
#define fabsf(x) __builtin_fabsf(x)
#define scalblnf(x, y) __builtin_scalblnf(x, y)
#define sqrtf(x) __builtin_sqrtf(x)
#endif

float hypotf(float x, float y)
{
    x = fabsf(x);
    y = fabsf(y);
    uint32_t ix = __float_bits(x);
    uint32_t iy = __float_bits(y);

    if (ix < iy) {
        ix = __float_bits(y);
        iy = __float_bits(x);
    }
    /* limit ration to something sensible:
     * If x/y > 2^25, x+y is different from x only in
     * non-nearest rounding, and so is hypot(x,y).
     */
    if (ix - iy > 25ul << 23) return x + y;
    int scale = 0;
    /* if x >= 2^64, then x² overflows. */
    if (ix >= (0x7f+64ul) << 23)
    {
        if (ix >= 0xfful << 23) {
            if (ix == 0xfful << 23 || iy == 0xfful << 23) return INFINITY;
            return NAN;
        }
        x = scalblnf(x, -100);
        y = scalblnf(y, -100);
        scale = 100;
    } else if (iy < (0x7f-64ul) << 23) {
        x = scalblnf(x, 100);
        y = scalblnf(y, 100);
        scale = -100;
    }
    float t = sqrtf(x*x+y*y);
    if (scale) t = scalblnf(t, scale);
    return t;
}

