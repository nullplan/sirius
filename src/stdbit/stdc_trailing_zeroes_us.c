#include <stdbit.h>
#include "cpu.h"
unsigned stdc_trailing_zeroes_us(unsigned short x) __unsequenced
{
    return x? a_ctz(x) : 16;
}
