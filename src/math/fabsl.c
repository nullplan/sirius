#include "libm.h"

long double fabsl(long double x)
{
    #if LDBL_MANT_DIG == 53
    return fabs(x);
    #else
    union ldshape ldx = {x};
    ldx.i.se &= 0x7fff;
    return ldx.f;
    #endif
}
