#include <complex.h>

complex long double cacosl(complex long double z)
{
    long double a = creall(z);
    long double b = cimagl(z);
    complex long double radicand = CMPLXL((a - b) * (a + b) - 1, 2*a*b);
    complex long double log = clogl(z + csqrtl(radicand));
    return CMPLXL(cimagl(log), -creall(log));
}
