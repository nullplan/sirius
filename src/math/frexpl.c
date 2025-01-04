#include "libm.h"

long double frexpl(long double x, int *exp)
{
#if LDBL_MANT_DIG == 53
    return frexp(x, exp);
#else
    union ldshape lds = {x};
    int e = lds.i.se & 0x7fff;
    if (e == 0x7fff) {
        *exp = 0;
        return x;
    }
    if (e == 0) {
        if (!x) {
            *exp = 0;
            return x;
        }
        lds.f *= 0x1p128L;
        e = (lds.i.se & 0x7fff) - 128;
    }
    *exp = e - 0x3ffe;
    lds.i.se &= 0x8000;
    lds.i.se |= 0x3ffe;
    return lds.f;
#endif
}
