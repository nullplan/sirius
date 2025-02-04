#include <complex.h>
#include <math.h>

complex double csqrt(complex double z)
{
    double a = creal(z);
    double b = cimag(z);
    if (a == 0 && b == 0) return 0;
    if (isinf(b)) return CMPLX(INFINITY, b);
    if (isinf(a) && isnan(b)) return signbit(a)? CMPLX(b, a) : z;

    double t = hypot(a, b);
    double x, y;
    if (signbit(a)) {
        y = sqrt(0.5 * (t - a));
        x = 0.5 * (b / y);
    } else {
        x = sqrt(0.5 * (t + a));
        y = 0.5 * (b / x);
    }
    return CMPLX(x, y);
}
