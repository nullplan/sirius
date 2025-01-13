#include "libm.h"

float scalblnf(float x, long n)
{
    if (n > 0x7f) {
        n -= 0x7f;
        x *= 0x1p127f;
        if (n > 0x7f) {
            n -= 0x7f;
            x *= 0x1p127f;
            if (n > 0x7f)
                n = 0x7f;
        }
    } else if (n < -0x7e) {
        x *= 0x1p-102f;
        n += 102;
        if (n < -0x7e) {
            x *= 0x1p-102f;
            n += 102;
            if (n < -0x7e)
                n = -0x7e;
        }
    }
    return x * __float_from_bits((n + 0x7f) << 23);
}

float scalbnf(float x, int n)
{
    return scalblnf(x, n);
}
weak_alias(ldexpf, scalbnf);
