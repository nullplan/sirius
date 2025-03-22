#include "libm.h"

#ifdef __GNUC__
#define fabs(x) __builtin_fabs(x)
#endif

double sin(double x)
{
    if (!isfinite(x)) return x - x;
    if (fabs(x) <= 0x1.921fb54442d18P-1) /* π/4 */
        return __kernel_sin(x, 0, 1);

    struct rempio2 p = __rem_pio2(x);
    switch (p.n & 3) {
        case 0: return +__kernel_sin(p.val[0], p.val[1], 0);
        case 1: return +__kernel_cos(p.val[0], p.val[1]);
        case 2: return -__kernel_sin(p.val[0], p.val[1], 0);
        default:return -__kernel_cos(p.val[0], p.val[1]);
    }
}
