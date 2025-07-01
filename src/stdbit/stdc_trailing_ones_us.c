#include <stdbit.h>
#include "cpu.h"

unsigned stdc_trailing_ones_us(unsigned short x) __unsequenced
{
    return a_ctz(~x);
}
