#include <complex.h>

complex long double casinhl(complex long double x)
{
    return clogl(x + csqrtl(x*x + 1));
}
