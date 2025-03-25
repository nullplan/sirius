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
    static const double atanhi[] = {
        0x1.dac670561bb4fP-2,           /* atan(1/2) */
        0x1.921fb54442d18P-1,           /* atan(1) = π/4 */
        0x1.f730bd281f69bP-1,           /* atan(1.5) */
        0x1.921fb54442d18P0,            /* π/2 */
    };
    static const double atanlo[] = {
        0x1.a2b7f222f65e2P-56,          /* atan(1/2) */
        0x1.1a62633145c07P-55,          /* atan(1) = π/4 */
        0x1.007887af0cbbdP-54,          /* atan(1.5) */
        0x1.1a62633145c07P-54,          /* π/2 */
    };
    int ex = ((__double_bits(x) >> 52) & 0x7ff) - 0x3ff;
    if (ex >= 66) {
        if (isnan(x)) return x;
        return copysign(atanhi[3] + atanlo[3], x);
    }
    if (ex < -29) {
        FORCE_EVAL(0x1p54 + x);
        return x;
    }
    int flip = signbit(x);
    x = fabs(x);
    int id;
    if (x < 0.4375)
        id = -1;
    else if (x < 0.6875) {
        id = 0;
        x = (2.0 * x - 1.0)/(2.0 + x);
    } else if (x < 1.1875) {
        id = 1;
        x = (x - 1.0)/(x + 1.0);
    } else if (x < 2.4375) {
        id = 2;
        x = (x - 1.5)/(1.0 + 1.5 * x);
    } else {
        id = 3;
        x = -1.0/x;
    }

    double z = x * x, w = z * z;
    double s1 = z * (c[0] + w * (c[2] + w * (c[4] + w * (c[6] + w * (c[8] + w * c[10])))));
    double s2 = w * (c[1] + w * (c[3] + w * (c[5] + w * (c[7] + w * c[9]))));
    if (id < 0) z = x - x * (s1 + s2);
    else z = atanhi[id] - (x * (s1 + s2) - atanlo[id] - x);
    if (flip) z = -z;
    return z;
}
