#include "libm.h"

int totalorder(const double *pa, const double *pb)
{
    int64_t ia, ib;
    ia = __double_bits(*pa);
    ib = __double_bits(*pb);
    return ia < 0 && ib < 0? ib <= ia : ia <= ib;
}
