#include "libm.h"

long double copysignl(long double abs, long double sgn)
{
    #if LDBL_MANT_DIG==53
    return copysign(abs, sgn);
    #else
    union ldshape lda = {abs}, lds = {sgn};
    lda.i.se &= 0x7fff;
    lda.i.se |= lds.i.se & 0x8000;
    return lda.f;
    #endif
}
