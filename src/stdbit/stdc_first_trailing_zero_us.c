#include <stdbit.h>
#include "cpu.h"

unsigned stdc_first_trailing_zero_us(unsigned short x) __unsequenced
{
    return x == 0xffff? 0 : a_ctz(~x) + 1;
}
