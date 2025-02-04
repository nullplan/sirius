#include <complex.h>
#include <math.h>

complex long double csqrtl(complex long double z)
{
    long double a = creall(z);
    long double b = cimagl(z);
    if (a == 0 && b == 0) return 0;
    if (isinf(b)) return CMPLXL(INFINITY, b);
    if (isinf(a) && isnan(b)) return signbit(a)? CMPLXL(b, a) : z;

    long double t = hypotl(a, b);
    long double x, y;
    if (signbit(a)) {
        y = sqrtl(0.5 * (t - a));
        x = 0.5 * (b / y);
    } else {
        x = sqrtl(0.5 * (t + a));
        y = 0.5 * (b / x);
    }
    return CMPLXL(x, y);
}
