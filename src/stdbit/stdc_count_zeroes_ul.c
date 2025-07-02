#include <stdbit.h>
#include "cpu.h"

unsigned stdc_count_zeroes_ul(unsigned long x) __unsequenced
{
    return a_popcnt(~x);
}
