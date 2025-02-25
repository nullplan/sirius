#include <complex.h>

/* see doc/csin */
complex double ccos(complex double z)
{
    complex double iz = CMPLX(-cimag(z), creal(z));
    return 0.5 * (cexp(iz) + cexp(-iz));
}

