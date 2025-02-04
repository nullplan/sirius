#include <complex.h>
#include <math.h>

double carg(complex double z)
{
    return atan2(cimag(z), creal(z));
}
