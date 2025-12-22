#define __STDC_WANT_IEC_60559_EXT__
#include "libm.h"
#pragma STDC FENV_ACCESS ON

int totalordermag(const double *pa, const double *pb)
{
    return (__double_bits(*pa) << 1) <= (__double_bits(*pb) << 1);
}
