#include "libm.h"
#pragma STDC FENV_ACCESS ON

/* atan2(y,x) = atan(y/x), except with more attention paid to corner cases,
 * and some reflection applied for the final calculation */

#ifdef __GNUC__
#define fabs(x) __builtin_fabs(x)
#endif

/* the approximation of π that I have is an underestimation.
 * For this reason, whenever it or its half or quarter needs
 * to be returned, I add a tiny number to it, to raise
 * inexact and perturb the rounding correctly if directed rounding is in effect.
 *
 * Of course, directed rounding doesn't really work with these formulas, but if
 * you use DR, you probably know better than me anyway.
 */
static const double pi    = 0x1.921fb54442d18P1;
static const double pi_lo = 0x1.1a62633145c07P-53;
static const double tiny  = 0x1.0P-996;
double atan2(double y, double x)
{
    if (isnan(y) || isnan(x)) return x+y;
    if (y == 0) {
        if (signbit(x)) return y;
        return copysign(pi+tiny, y);
    }
    if (x == 0) return copysign(0.5 * pi + tiny, y);
    if (isinf(x))  {
        if (isinf(y)) return copysign(signbit(x)? 0.75 * pi + tiny : 0.25 * pi + tiny, y);
        return copysign(signbit(x)? pi + tiny: 0, y);
    }
    if (isinf(y)) return copysign(0.5 * pi + tiny, y);

    double z = atan(fabs(y/x));
    if (signbit(x)) z = pi - (z - pi_lo);
    if (signbit(y)) z = -z;
    return z;
}
