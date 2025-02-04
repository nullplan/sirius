#include <complex.h>

complex float conjf(complex float z)
{
    return CMPLXF(crealf(z), -cimagf(z));
}
