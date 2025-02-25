#include <complex.h>

complex double ctan(complex double z)
{
    complex double iz = CMPLX(-cimag(z), creal(z));
    complex double eiz = cexp(iz);
    complex double emiz = cexp(-iz);
    z = (eiz - emiz)/(eiz + emiz);
    return CMPLX(cimag(z), -creal(z));
}
