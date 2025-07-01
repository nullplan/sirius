#include <stdbit.h>
#include "cpu.h"
unsigned stdc_trailing_zeroes_uc(unsigned char x) __unsequenced
{
    return x? a_ctz(x) : 8;
}
