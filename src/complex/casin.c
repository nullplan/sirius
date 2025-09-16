#include <complex.h>

complex double casin(complex double z)
{
    double a = creal(z);
    double b = cimag(z);
    complex double radicand = CMPLX(1 - a*a + b*b, -2*a*b);
    complex double log = clog(CMPLX(-b, a) + csqrt(radicand));
    return CMPLX(cimag(log), -creal(log));
}
