#include <complex.h>

complex float  ctanhf(complex float x)
{
    complex float ex = cexpf(x);
    complex float emx = cexpf(-x);
    return (ex - emx)/(ex + emx);
}
