#include <complex.h>
#include <math.h>
/* exp(a+bi) = exp(a) exp(bi) = exp(a) (cos(b) + i sin(b)) */

complex double cexp(complex double z)
{
    return exp(creal(z)) * CMPLX(cos(cimag(z)), sin(cimag(z)));
}
