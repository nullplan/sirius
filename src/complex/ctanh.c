#include <complex.h>

complex double  ctanh(complex double x)
{
    complex double ex = cexp(x);
    complex double emx = cexp(-x);
    return (ex - emx)/(ex + emx);
}
