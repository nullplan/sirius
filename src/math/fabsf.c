#include "libm.h"

float fabsf(float x)
{
    return __float_from_bits(__float_bits(x) << 1 >> 1);
}
