#include <stdbit.h>
#include "cpu.h"

unsigned stdc_leading_ones_us(unsigned short x) __unsequenced
{
    return a_clz(~((size_t)x << (8 * (sizeof (size_t) - 2))));
}
