#include "libm.h"

int __signbitl(long double x)
{
#if LDBL_MANT_DIG == 53
    return __double_bits(x) >> 63;
#else
    union ldshape lds = {x};
    return lds.i.se >> 15;
#endif
}
