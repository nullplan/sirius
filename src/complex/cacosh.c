#include <complex.h>

complex double cacosh(complex double x)
{
    return clog(x + csqrt(x*x - 1));
}
