#include <complex.h>
#include <math.h>

/* ln(r exp(iφ)) = ln r + iφ */
complex float clogf(complex float z)
{
    return CMPLXF(logf(cabsf(z)), cargf(z));
}
