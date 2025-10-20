#include <complex.h>

complex long double  ctanhl(complex long double x)
{
    complex long double ex = cexpl(x);
    complex long double emx = cexpl(-x);
    return (ex - emx)/(ex + emx);
}
