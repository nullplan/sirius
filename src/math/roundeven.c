#include "libm.h"
#pragma STDC FENV_ACCESS ON

double roundeven(double x)
{
    uint64_t ix = __double_bits(x);
    int exp = ((ix >> 52) & 0x7ff) - 0x3ff;
    /* trivial case: all bits fractional (also catches subnormals and zero) */
    if (exp < 0) {
        FORCE_EVAL(1e300 + x); /* raise inexact if x != 0 */
        /* ties resolved to even, which is 0 in this case */
        if (ix << 1 <= 0x3feULL << 53) return __double_from_bits(ix >> 63 << 63);
        return ix >> 63? -1 : 1;
    }
    /* trivial case: no bits fractional */
    if (exp >= 52) return x;
    /* some fractional bits? */
    uint64_t fractmask = ((1ull << 52) - 1) >> exp;
    ix += fractmask >> 1;
    ix += (ix >> (53 - exp)) & 1;
    double y = __double_from_bits(ix & ~fractmask);
    FORCE_EVAL(1e300 + (x - y)); /* raise inexact if x != y. Note that |x-y| < 1 */
    return y;
}
