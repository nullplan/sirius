#include "libm.h"

double scalbln(double x, long n)
{
    if (n > 0x3ff) {
        x *= 0x1p1023;
        n -= 0x3ff;
        if (n > 0x3ff) {
            x *= 0x1p1023;
            n -= 0x3ff;
            if (n > 0x3ff)
                n = 0x3ff;
        }
    } else if (n < -0x3fe) {
        x *= 0x1p-970;
        n += 970;
        if (n < -0x3fe) {
            x *= 0x1p-970;
            n += 970;
            if (n < -0x3fe)
                n = -0x3fe;
        }
    }
    return x * __double_from_bits((n + 0x3ffull) << 52);
}

double scalbn(double x, int n)
{
    return scalbln(x, n);
}
weak_alias(ldexp, scalbn);
