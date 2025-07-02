#include <stdbit.h>
#include "cpu.h"

unsigned stdc_count_ones_us(unsigned short x) __unsequenced
{
    return a_popcnt(x);
}
