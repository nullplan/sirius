#include <stdbit.h>
#include "cpu.h"

unsigned stdc_first_leading_zero_us(unsigned short x) __unsequenced
{
    return x == 0xffff? 0 : a_clz(~((size_t)x << (8 * (sizeof (size_t) - 2)))) + 1;
}
