#include <stdbit.h>
#include "cpu.h"

unsigned stdc_count_zeroes_us(unsigned short x) __unsequenced
{
    return a_popcnt(~x) - 8 * (sizeof (size_t) - 2);
}
