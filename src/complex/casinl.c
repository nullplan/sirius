#include <complex.h>

complex long double casinl(complex long double z)
{
    long double a = creall(z);
    long double b = cimagl(z);
    complex long double radicand = CMPLXL(1 - a*a + b*b, -2*a*b);
    complex long double log = clogl(CMPLXL(-b, a) + csqrtl(radicand));
    return CMPLXL(cimagl(log), -creall(log));
}
