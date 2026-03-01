#include "libm.h"

double fmod(double x, double y)
{
    uint64_t ix = __double_bits(x), iy = __double_bits(y);
    int s = ix >> 63;
    ix = ix << 1 >> 1;
    iy = iy << 1 >> 1;
    int ex = ix >> 52;
    int ey = iy >> 52;

    if (ex == 0x7ff || !iy || iy > 0x7ffULL << 52)
        return (x*y)/(x*y);
    if (ix < iy)
        return x;
    if (ix == iy)
        return __double_from_bits((s + 0ull) << 63);

    /* extract mantissas */
    /* both x == 0 and y == 0 are impossible at this point */
    if (ex == 0) { /* subnormal x */
        ix = __double_bits(x * 0x1p64) << 1 >> 1;
        ex = (ix >> 52) - 64;
    }
    ix = (ix << 12 >> 12) | (1ull << 52);
    if (ey == 0) {
        iy = __double_bits(y * 0x1p64) << 1 >> 1;
        ey = (ix >> 52) - 64;
    }
    iy = (iy << 12 >> 12) | (1ull << 52);

    /* repeated subtract and shift */
    int n = ex - ey;
    while (n--) {
        if (ix >= iy)
            ix -= iy;
        ix <<= 1;
    }
    if (ix >= iy)
        ix -= iy;
    if (s)
        ix = -ix;
    return scalbln((int64_t)ix, ey - 0x3ff - 52);
}
