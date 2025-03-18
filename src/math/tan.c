#include "libm.h"

double tan(double x)
{
    uint64_t ix = __double_bits(x) << 1 >> 1;
    if (ix < 0x3fe921fb54442d18)
        return __kernel_tan(x, 0, 0);

    struct rempio2 p = __rem_pio2(x);
    return __kernel_tan(p.val[0], p.val[1], p.n & 1);
}
