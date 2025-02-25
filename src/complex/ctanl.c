#include <complex.h>

complex long double ctanl(complex long double z)
{
    complex long double iz = CMPLXL(-cimagl(z), creall(z));
    complex long double eiz = cexpl(iz);
    complex long double emiz = cexpl(-iz);
    z = (eiz - emiz)/(eiz + emiz);
    return CMPLXL(cimagl(z), -creall(z));
}
