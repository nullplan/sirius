#include "libm.h"
#pragma STDC FENV_ACCESS ON

double scalbln(double x, long n)
{
    if (unlikely(n > 0x3ff)) {
        x *= 0x1p1023;
        n -= 0x3ff;
        if (n > 0x3ff) {
            x *= 0x1p1023;
            n -= 0x3ff;
            if (n > 0x3ff)
                n = 0x3ff;
        }
    } else if (unlikely(n < -0x3fe)) {
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
