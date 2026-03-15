#include "libm.h"

#ifdef __GNUC__
#define copysign __builtin_copysign
#endif

double nextafter(double x, double y)
{
    uint64_t ix = __double_bits(x);
    uint64_t iy = __double_bits(y);

    if (ix << 1 > 0x7ffULL << 53 || iy << 1 > 0x7ffULL << 53)
        return x + y;

    if (ix == iy || ix << 1 > 0x7ffULL << 53)
        return x;
    /* if signs differ, in the main, reduce representation by 1 (toward zero) */
    /* if signs are equal, comparison of the representations equals the comparison of the values */
    if ((ix ^ iy) >> 63 || ix > iy)
    {
        /* except if exp == mant == 0, that does the wrong thing. */
        if (ix == 0) return -DBL_TRUE_MIN;
        if (!(ix << 1)) return DBL_TRUE_MIN;

        /* if the result is subnormal or zero, we must raise inexact and underflow */
        if (ix << 1 <= 1ULL << 53)
            return x - copysign(DBL_TRUE_MIN, x);
        /* otherwise just reduce representation by 1 */
        return __double_from_bits(ix - 1);
    }
    
    /* so now we must raise the representation. If x == DBL_MAX, we must raise overflow. */
    if (ix << 1 == (0x7ffULL << 53) - 2)
        return x + copysign(0x1p971, x);

    /* also, we must still raise underflow if the result remains subnormal */
    if (ix << 1 < 1ULL << 52 - 1)
        return x + copysign(DBL_TRUE_MIN, x);
    return __double_from_bits(ix+1);

}
