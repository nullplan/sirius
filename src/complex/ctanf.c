#include <complex.h>

complex float ctanf(complex float z)
{
    complex float iz = CMPLXF(-cimagf(z), crealf(z));
    complex float eiz = cexpf(iz);
    complex float emiz = cexpf(-iz);
    z = (eiz - emiz)/(eiz + emiz);
    return CMPLXF(cimagf(z), -crealf(z));
}
