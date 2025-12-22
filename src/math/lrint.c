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
long lrint(double x)
{
    return rint(x);
}
#else
/* Otherwise, we can assume LONG_MAX == 2³¹-1, which has the double representation
 * 0x41df_ffff_ffc0_0000, so anything with a larger representation than that can overflow.
 */
long lrint(double x)
{
    uint64_t ix = __double_bits(x) << 1 >> 1;
    if (ix <= 0x41dfffffffc00000ULL) return rint(x);
    int except = fetestexcept(FE_INEXACT | FE_OVERFLOW);
    long y = rint(x);
    if (!except && (fetestexcept(FE_INEXACT | FE_OVERFLOW) == (FE_INEXACT | FE_OVERFLOW)))
        feclearexcept(FE_INEXACT);
    return y;
}
#endif
