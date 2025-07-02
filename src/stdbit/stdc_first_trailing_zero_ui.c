#include <stdbit.h>
#include "cpu.h"

unsigned stdc_first_trailing_zero_ui(unsigned x) __unsequenced
{
    return x == 0xffffffff? 0 : a_ctz(~x) + 1;
}
