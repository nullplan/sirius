#include <complex.h>
#include <math.h>

complex float cprojf(complex float z)
{
    if (isinf(crealf(z)) || isinf(cimagf(z)))
        return CMPLXF(INFINITY, copysignf(0, cimagf(z)));
    return z;
}
