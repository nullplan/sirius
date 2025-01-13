#include "libm.h"

long double scalblnl(long double x, long n)
{
#if LDBL_MANT_DIG==53
    return scalbln(x, n);
#else
    union ldshape ldn;
    #if LDBL_MANT_DIG==64
    ldn.i.mant = 1ull << 63;
    #else
    ldn.i.mantlo = ldn.i.mantmid = ldn.i.manthi = 0;
    #endif
    if (n > 0x3fff) {
        n -= 0x3fff;
        x *= 0x1p16383L;
        if (n > 0x3fff) {
            n -= 0x3fff;
            x *= 0x1p16383L;
            if (n > 0x3fff)
                n = 0x3fff;
        }
    } else if (n < -0x3ffe) {
        n += 16255;
        x *= 0x1p-16255L;
        if (n < -0x3ffe) {
            n += 16255;
            x *= 0x1p-16255L;
            if (n < -0x3ffe)
                n = -0x3ffe;
        }
    }
    ldn.i.se = n + 0x3fff;
    return x * ldn.f;
#endif
}

long double scalbnl(long double x, int n)
{
    return scalblnl(x, n);
}
weak_alias(ldexpl, scalbnl);
