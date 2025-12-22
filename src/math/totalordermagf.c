#define __STDC_WANT_IEC_60559_EXT__
#include "libm.h"
#pragma STDC FENV_ACCESS ON

int totalordermagf(const float *pa, const float *pb)
{
    return (__float_bits(*pa) << 1) <= (__float_bits(*pb) << 1);
}
