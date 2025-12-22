#include "libm.h"
#pragma STDC FENV_ACCESS ON

double ceil(double x)
{
    uint64_t ix = __double_bits(x);
    int exp = ((ix >> 52) & 0x7ff) - 0x3ff;
    /* trivial case: all bits fractional (also catches subnormals and zero) */
    if (exp < 0) {
        FORCE_EVAL(1e300 + x); /* raise inexact if x != 0 */
        if (!(ix << 1)) return x;
        return ix >> 63? 0 : 1;
    }
    /* trivial case: no bits fractional */
    if (exp >= 52) return x;
    /* some fractional bits? */
    uint64_t fractmask = ((1ull << 52) - 1) >> exp;
    if (!(ix >> 63)) ix += fractmask;
    double y = __double_from_bits(ix & ~fractmask);
    FORCE_EVAL(1e300 + (x - y)); /* raise inexact if x != y. Note that |x-y| < 1 */
    return y;
}
