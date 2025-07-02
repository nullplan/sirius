#include <stdbit.h>
#include "cpu.h"

unsigned stdc_first_trailing_zero_uc(unsigned char x) __unsequenced
{
    return x == 0xff? 0 : a_ctz(~x) + 1;
}
