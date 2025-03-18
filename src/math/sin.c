#include "libm.h"

double sin(double x)
{
    if (!isfinite(x)) return x - x;
    uint64_t ix = __double_bits(x) << 1 >> 1;
    if (ix < 0x3fe921fb54442d18)
        return __kernel_sin(x, 0, 1);

    struct rempio2 p = __rem_pio2(x);
    switch (p.n & 3) {
        case 0: return +__kernel_sin(p.val[0], p.val[1], 0);
        case 1: return +__kernel_cos(p.val[0], p.val[1]);
        case 2: return -__kernel_sin(p.val[0], p.val[1], 0);
        default:return -__kernel_cos(p.val[0], p.val[1]);
    }
}
