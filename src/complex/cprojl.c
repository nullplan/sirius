#include <complex.h>
#include <math.h>

complex long double cprojl(complex long double z)
{
    if (isinf(creall(z)) || isinf(cimagl(z)))
        return CMPLXL(INFINITY, copysignl(0, cimagl(z)));
    return z;
}
