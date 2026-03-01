#include "libm.h"

double modf(double x, double *iptr)
{
    if (isnan(x)) {
        *iptr = x;
        return x;
    }

    int exp = ((__double_bits(x) >> 52) & 0x7ff) - 0x3ff;
    /* first simple case: radix behind the digits, so the number already is an integer. */
    if (exp >= 52) {
        *iptr = x;
        return copysign(0, x);
    }
    /* second simple case: radix in front of the digits, so the integer part is 0 */
    if (exp < 0) {
        *iptr = 0;
        return x;
    }
    /* finally, radix in the middle of the digits. You get the integer by clearing out the post-radix bits. */
    uint64_t fractmask = (1ull << (52 - exp)) - 1;
    double r = *iptr = __double_from_bits(__double_bits(x) & ~fractmask);
    return x - r; /* and the fractional part is just the difference */
}
