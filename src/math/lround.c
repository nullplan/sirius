#include "libm.h"
#include <limits.h>
#include <fenv.h>
#pragma STDC FENV_ACCESS ON

/* This can just return the integer-rounded result of rint() so long as not both
 * overflow and inexact exceptions can happen.
 * On 64-bit systems, LONG_MAX is in the range where no fractional bits remain, so
 * an inexact exception is impossible in the range where an overflow would happen.
 * Also, on softfloat systems that don't have either overflow or inexact exceptions,
 * it is obviously also impossible for both to happen at the same time.
 */
#if !defined FE_OVERFLOW || !defined FE_INEXACT || LONG_MAX > (1U << 52)
long lround(double x)
{
    return round(x);
}
#else
/* Otherwise, we can assume LONG_MAX == 2³¹-1. Anything that is at least
 * (2³¹-1) + 1/2 WILL overflow, which has the double representation of
 * 0x41df_ffff_ffe0_0000.
 */
long lround(double x)
{
    uint64_t ix = __double_bits(x) << 1 >> 1;
    if (ix < 0x41dfffffffe00000ULL) return round(x);
    int except = fetestexcept(FE_INEXACT);
    long y = round(x);
    if (!except)
        feclearexcept(FE_INEXACT);
    return y;
}
#endif
