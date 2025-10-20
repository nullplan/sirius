#include <complex.h>

complex long double  ccoshl(complex long double x)
{
    return 0.5 * (cexpl(x) + cexpl(-x));
}
