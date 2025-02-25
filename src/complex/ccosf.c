#include <complex.h>

/* see doc/csin */
complex float ccosf(complex float z)
{
    complex float iz = CMPLXF(-cimagf(z), crealf(z));
    return 0.5 * (cexpf(iz) + cexpf(-iz));
}

