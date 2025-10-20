#include <complex.h>

complex float catanf(complex float z)
{
    float a = crealf(z);
    float b = cimagf(z);
    complex float zsq = CMPLXF(a*a - b*b, 2*a*b);
    complex float twozi = 2 * CMPLXF(-b, a);
    complex float log = clogf(-zsq + twozi + 1) - clogf(zsq + 1);
    return -0.5 * CMPLXF(-cimagf(log), crealf(log));
}
