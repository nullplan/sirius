#include <complex.h>
#include <math.h>

complex float csqrtf(complex float z)
{
    float a = crealf(z);
    float b = cimagf(z);
    if (a == 0 && b == 0) return 0;
    if (isinf(b)) return CMPLXF(INFINITY, b);
    if (isinf(a) && isnan(b)) return signbit(a)? CMPLXF(b, a) : z;

    float t = hypotf(a, b);
    float x, y;
    if (signbit(a)) {
        y = sqrtf(0.5 * (t - a));
        x = 0.5 * (b / y);
    } else {
        x = sqrtf(0.5 * (t + a));
        y = 0.5 * (b / x);
    }
    return CMPLXF(x, y);
}
