#include "libm.h"

/* For the most part, I just use the Taylor series up to degree 23 here.
 *
 * But the Taylor series is most precise around zero, so I use a couple
 * of reflection formulas derived from the arctangent addition formula:
 *
 * arctan u + arctan v = arctan ((u + v)/(1 - uv))
 *
 * Therefore if I fix u, I can find some v such that
 * the argument on the RHS is x.
 *
 * x = (u + v)/(1 - uv)
 * (1 - uv) x = u + v
 * x - uvx = u + v
 * x - u = v + uvx
 * x - u = v (1 + ux)
 * (x - u)/(1 + ux) = v
 *
 * So we use the formula:
 *
 * arctan x = arctan a + arctan ((x - a)/(1 + ax)) for some a.
 *
 * Also one more thing: Since tan (x + π/2) = -1/tan x, we have arctan x = π/2 + arctan (-1/x),
 * which we can use for large x.
 *
 * We use the series up to degree 23, so the error is O(x²⁴).
 */

double atan(double x)
{
    static const double c[] = {
        +1.0/3.0,
        -1.0/5.0,
        +1.0/7.0,
        -1.0/9.0,
        +1.0/11.0,
        -1.0/13.0,
        +1.0/15.0,
        -1.0/17.0,
        +1.0/19.0,
        -1.0/21.0,
        +1.0/23.0,
    };
    int ex = ((__double_bits(x) >> 52) & 0x7ff) - 0x3ff;
    if (ex >= 66) {
        if (isnan(x)) return x;
        FORCE_EVAL(0x1p54 + 0x1p-3);
        return copysign(M_PI_2, x);
    }
    if (ex < -29) {
        FORCE_EVAL(0x1p54 + x);
        return x;
    }
    int flip = signbit(x);
    if (flip) x = -x;
    int addconst = 1;
    double hiconst;
    double loconst;
    if (x < 0.4375)
        addconst = 0;
    else if (x < 0.6875) {
        hiconst = 0x1.dac670561bb4fP-2;           /* atan(1/2) */
        loconst = 0x1.a2b7f222f65e2P-56;          /* atan(1/2) */
        x = (x - 0.5)/(1.0 + 0.5 * x);
    } else if (x < 1.1875) {
        hiconst = 0x1.921fb54442d18P-1;           /* atan(1) = π/4 */
        loconst = 0x1.1a62633145c07P-55;          /* atan(1) = π/4 */
        x = (x - 1.0)/(x + 1.0);
    } else if (x < 2.4375) {
        hiconst = 0x1.f730bd281f69bP-1;           /* atan(1.5) */
        loconst = 0x1.007887af0cbbdP-54;          /* atan(1.5) */
        x = (x - 1.5)/(1.0 + 1.5 * x);
    } else {
        hiconst = 0x1.921fb54442d18P0;            /* π/2 */
        loconst = 0x1.1a62633145c07P-54;          /* π/2 */
        x = -1.0/x;
    }

    double x2 = x * x, x4 = x2 * x2;
    double s1 = x2 * (c[0] + x4 * (c[2] + x4 * (c[4] + x4 * (c[6] + x4 * (c[8] + x4 * c[10])))));
    double s2 = x4 * (c[1] + x4 * (c[3] + x4 * (c[5] + x4 * (c[7] + x4 * c[9]))));
    if (!addconst) x = x - x * (s1 + s2);
    else x = hiconst - (x * (s1 + s2) - loconst - x);
    if (flip) x = -x;
    return x;
}
