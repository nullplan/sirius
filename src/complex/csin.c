#include <complex.h>

/* see doc/csin */
complex double csin(complex double z)
{
    complex double iz = CMPLX(-cimag(z), creal(z));
    z = cexp(iz) - cexp(-iz);
    return 0.5 * CMPLX(cimag(z), -creal(z));
}
