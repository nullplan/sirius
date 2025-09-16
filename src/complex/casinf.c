#include <complex.h>

complex float casinf(complex float z)
{
    float a = crealf(z);
    float b = cimagf(z);
    complex float radicand = CMPLXF(1 - a*a + b*b, -2*a*b);
    complex float log = clogf(CMPLXF(-b, a) + csqrtf(radicand));
    return CMPLXF(cimagf(log), -crealf(log));
}
