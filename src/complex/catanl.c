#include <complex.h>

complex long double catanl(complex long double z)
{
    long double a = creall(z);
    long double b = cimagl(z);
    complex long double zsq = CMPLXL(a*a - b*b, 2*a*b);
    complex long double twozi = 2 * CMPLXL(-b, a);
    complex long double log = clogl(-zsq + twozi + 1) - clogl(zsq + 1);
    return -0.5 * CMPLXL(-cimagl(log), creall(log));
}
