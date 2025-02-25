#include <complex.h>
#include <math.h>
/* exp(a+bi) = exp(a) exp(bi) = exp(a) (cos(b) + i sin(b)) */

complex float cexpf(complex float z)
{
    return expf(crealf(z)) * CMPLXF(cosf(cimagf(z)), sinf(cimagf(z)));
}
