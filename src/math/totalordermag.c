#include "libm.h"

int totalordermag(const double *pa, const double *pb)
{
    return (__double_bits(*pa) << 1) <= (__double_bits(*pb) << 1);
}
