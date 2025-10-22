#include <complex.h>

complex double casinh(complex double x)
{
    return clog(x + csqrt(x*x + 1));
}
