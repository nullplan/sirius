#include <stdbit.h>
#include "cpu.h"

unsigned stdc_count_ones_ull(unsigned long long x) __unsequenced
{
    return a_popcnt_64(x);
}
