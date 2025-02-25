#include "libm.h"

float copysignf(float abs, float sgn)
{
    return __float_from_bits((__float_bits(abs) << 1 >> 1) | (__float_bits(sgn) >> 31 << 31));
}
