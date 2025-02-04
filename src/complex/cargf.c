#include <complex.h>
#include <math.h>

float cargf(complex float z)
{
    return atan2f(cimagf(z), crealf(z));
}
