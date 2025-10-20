#include <complex.h>

complex double  catan(complex double z)
{
    double a = creal(z);
    double b = cimag(z);
    complex double zsq = CMPLX(a*a - b*b, 2*a*b);
    complex double twozi = 2 * CMPLX(-b, a);
    complex double log = clog(-zsq + twozi + 1) - clog(zsq + 1);
    return -0.5 * CMPLX(-cimag(log), creal(log));
}
