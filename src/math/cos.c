#include "libm.h"

double cos(double x)
{
    uint64_t ix = __double_bits(x) << 1 >> 1;
    if (ix < 0x3fe921fb54442d18)
        return __kernel_cos(x, 0);

    struct rempio2 p = __rem_pio2(x);
    switch (p.n & 3) {
        case 0: return +__kernel_cos(p.val[0], p.val[1]);
        case 1: return -__kernel_sin(p.val[0], p.val[1], 0);
        case 2: return -__kernel_cos(p.val[0], p.val[1]);
        default:return +__kernel_sin(p.val[0], p.val[1], 0);
    }
}
