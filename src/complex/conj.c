#include <complex.h>

complex double conj(complex double z)
{
    return CMPLX(creal(z), -cimag(z));
}
