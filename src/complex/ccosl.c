#include <complex.h>

/* see doc/csin */
complex long double ccosl(complex long double z)
{
    complex double iz = CMPLXL(-cimagl(z), creall(z));
    return 0.5 * (cexpl(iz) + cexpl(-iz));
}

