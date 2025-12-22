#define __STDC_WANT_IEC_60559_EXT__
#include "libm.h"
#pragma STDC FENV_ACCESS ON

int    totalorderf(const float *pa, const float *pb)
{
    int32_t ia = __float_bits(*pa);
    int32_t ib = __float_bits(*pb);
    return ia < 0 && ib < 0? ib <= ia : ia <= ib;
}
