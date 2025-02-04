#include <complex.h>
#include <math.h>

float cabsf(complex float z)
{
    return hypotf(crealf(z), cimagf(z));
}
