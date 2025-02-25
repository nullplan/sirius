#include <complex.h>
#include <math.h>
/* exp(a+bi) = exp(a) exp(bi) = exp(a) (cos(b) + i sin(b)) */

complex long double cexpl(complex long double z)
{
    return expl(creall(z)) * CMPLXL(cosl(cimagl(z)), sinl(cimagl(z)));
}
