#include <complex.h>

complex double  csinh(complex double x)
{
    return 0.5 * (cexp(x) - cexp(-x));
}
