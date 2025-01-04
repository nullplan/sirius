#include "libm.h"

double frexp(double x, int *exp)
{
    uint64_t i = __double_bits(x);
    int e = (i >> 52) & 0x7ff;
    if (e == 0x7ff) {
        *exp = 0;
        return x;
    }
    if (e == 0) {
        if (!x) {
            *exp = 0;
            return x;
        }
        x *= 0x1p64;
        i = __double_bits(x);
        e = ((i >> 52) & 0x7ff) - 64;
    }
    *exp = e - 0x3fe;
    i &= 0x800fffffffffffffULL;
    i |= 0x3fe0000000000000ULL;
    return __double_from_bits(i);
}
