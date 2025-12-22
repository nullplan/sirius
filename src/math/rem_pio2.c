/* A simpler design than what musl/fdlibm are doing.
 * Not as accurate, but defensible, IMHO.
 *
 * The multi-precision formulas were adopted from the paper
 * Shewchuk, "Arbitrary Precision Floating-Point Arithmetic", Carnegie Mellon University, Pittsburgh, 1997
 */
#include "libm.h"
#pragma STDC FENV_ACCESS ON
static const double halfpi = 0x1.921fb54442d18p0;

struct pair {
    double hi, lo;
};

static struct pair split(double x)
{
    double c = 0x1.0000002p27 * x;
    double big = c - x;
    double hi = c - big;
    double lo = x - hi;
    return (struct pair){hi, lo};
}

static struct pair twoprod(double a, double b)
{
    double x = a * b;
    struct pair as = split(a);
    struct pair bs = split(b);
    double err = x - (as.hi * bs.hi) - (as.lo * bs.hi) - (as.hi * bs.lo);
    double y = as.lo * bs.lo - err;
    return (struct pair){x, y};
}

static struct pair twosum(double a, double b)
{
    double x = a + b;
    double bv = x - a;
    double av = x - bv;
    double br = b - bv;
    double ar = a - av;
    double y = ar + br;
    return (struct pair){x, y};
}

#ifdef __GNUC__
#define fabs(x) __builtin_fabs(x)
#endif

hidden struct rempio2 __rem_pio2(double x)
{
    if (fabs(x) >= 8/DBL_EPSILON) /* ULP > 2π -> just return 0/0 */
        return (struct rempio2){0};
    double n = round(x/halfpi);

    if (fabs(x) >= 2/DBL_EPSILON) /* ULP > π/2 -> return n/0 */
        return (struct rempio2){(int64_t)n, {0}};

    struct pair mnhalfpi = twoprod(halfpi, -n);
    struct pair p = twosum(x, mnhalfpi.lo);
    struct pair q = twosum(p.hi, mnhalfpi.hi);
    if (q.hi) {
        if (q.lo)
            return (struct rempio2){(int64_t)n, {q.hi, q.lo}};
        else
            return (struct rempio2){(int64_t)n, {q.hi, p.lo}};
    } else if (q.lo)
        return (struct rempio2){(int64_t)n, {q.lo, p.lo}};
    else
        return (struct rempio2){(int64_t)n, {p.lo, 0}};
}
