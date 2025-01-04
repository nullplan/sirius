#include "libm.h"

float frexpf(float x, int *exp)
{
    uint32_t i = __float_bits(x);
    int e = (i >> 23) & 0xff;
    if (e == 0xff) {
        *exp = 0;
        return 0;
    }
    if (e == 0) {
        if (!x) {
            *exp = 0;
            return 0;
        }
        x *= 0x1p32f;
        i = __float_bits(x);
        e = ((i >> 23) & 0xff) - 32;
    }
    *exp = e - 0x7e;
    i &= 0x807fffff;
    i |= 0x3f000000;
    return __float_from_bits(i);
}
