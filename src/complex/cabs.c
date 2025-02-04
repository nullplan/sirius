#include <complex.h>
#include <math.h>

double cabs(complex double z)
{
    return hypot(creal(z), cimag(z));
}
