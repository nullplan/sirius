#include <stdbit.h>
#include "cpu.h"

unsigned stdc_trailing_ones_uc(unsigned char x) __unsequenced
{
    return a_ctz(~x);
}
