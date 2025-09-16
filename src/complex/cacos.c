#include <complex.h>

complex double cacos(complex double z)
{
    double a = creal(z);
    double b = cimag(z);
    complex double radicand = CMPLX((a - b) * (a + b) - 1, 2*a*b);
    complex double log = clog(z + csqrt(radicand));
    return CMPLX(cimag(log), -creal(log));
}
