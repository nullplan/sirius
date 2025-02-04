#include <complex.h>
#include <math.h>

complex double cproj(complex double z)
{
    if (isinf(creal(z)) || isinf(cimag(z)))
        return CMPLX(INFINITY, copysign(0, cimag(z)));
    return z;
}
