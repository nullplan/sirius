#include "libm.h"

double fabs(double x)
{
    return __double_from_bits(__double_bits(x) << 1 >> 1);
}
