#include "libm.h"

int __fpclassifyl(long double x)
{
#if LDBL_MANT_DIG==53
    return __fpclassifyd(__double_bits(x) << 1 >> 1);
#else
    union ldshape lds = {x};
    int e = lds.i.se & 0x7fff;
    if (e == 0) return mantzero(lds)? FP_ZERO : FP_SUBNORMAL;
    if (e == 0x7fff) return mantzero(lds)? FP_INFINITE : FP_NAN;
    return FP_NORMAL;
#endif
}
