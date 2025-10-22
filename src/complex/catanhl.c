#include <complex.h>

complex long double catanhl(complex long double x)
{
    return 0.5 * (clogl(1 + x) - clogl(1 - x));
}
