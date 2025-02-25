#include <complex.h>
#include <math.h>

/* ln(r exp(iφ)) = ln r + iφ */
complex long double clogl(complex long double z)
{
    return CMPLXL(logl(cabsl(z)), cargl(z));
}
