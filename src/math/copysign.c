#include "libm.h"

double copysign(double abs, double sgn)
{
    return __double_from_bits((__double_bits(abs) << 1 >> 1) | (__double_bits(sgn) >> 63 << 63));
}
