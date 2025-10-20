#include <complex.h>

complex double  ccosh(complex double x)
{
    return 0.5 * (cexp(x) + cexp(-x));
}
