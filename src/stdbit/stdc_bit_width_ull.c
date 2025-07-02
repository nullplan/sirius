#include <stdbit.h>
#include "cpu.h"

unsigned stdc_bit_width_ull(unsigned long long x) __unsequenced
{
    return x == 0? 0 : 64 - a_clz_64(x);
}
