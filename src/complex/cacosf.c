#include <complex.h>

complex float cacosf(complex float z)
{
    float a = crealf(z);
    float b = cimagf(z);
    complex float radicand = CMPLXF((a - b) * (a + b) - 1, 2*a*b);
    complex float log = clogf(z + csqrtf(radicand));
    return CMPLXF(cimagf(log), -crealf(log));
}
