#include <complex.h>
#include <math.h>

/* ln(r exp(iφ)) = ln r + iφ */
complex double   clog(complex double z)
{
    return CMPLX(log(cabs(z)), carg(z));
}
