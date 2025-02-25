#include <complex.h>

/* see doc/csin */
complex long double csinl(complex long double z)
{
    complex long double iz = CMPLXL(-cimagl(z), creall(z));
    z = cexpl(iz) - cexpl(-iz);
    return 0.5 * CMPLXL(cimagl(z), -creall(z));
}
