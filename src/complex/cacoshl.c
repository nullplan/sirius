#include <complex.h>

complex long double cacoshl(complex long double x)
{
    return clogl(x + csqrtl(x*x - 1));
}
