#include "libm.h"

#ifdef __GNUC__
#define fabs(x) __builtin_fabs(x)
#endif

double tan(double x)
{
    if (!isfinite(x)) return x - x;
    if (fabs(x) <= 0x1.921fb54442d18P-1) /* π/4 */
        return __kernel_tan(x, 0, 0);

    struct rempio2 p = __rem_pio2(x);
    return __kernel_tan(p.val[0], p.val[1], p.n & 1);
}
