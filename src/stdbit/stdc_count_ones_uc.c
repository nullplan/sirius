#include <stdbit.h>
#include "cpu.h"

unsigned stdc_count_ones_uc(unsigned char x) __unsequenced
{
    return a_popcnt(x);
}
