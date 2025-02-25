#include <complex.h>

/* see doc/csin */
complex float csinf(complex float z)
{
    complex float iz = CMPLXF(-cimagf(z), crealf(z));
    z = cexpf(iz) - cexpf(-iz);
    return 0.5 * CMPLXF(cimagf(z), -crealf(z));
}
