#include <stdbit.h>
#include "cpu.h"

unsigned stdc_first_trailing_zero_ul(unsigned long x) __unsequenced
{
    return x == -1ul? 0 : a_ctz(~x) + 1;
}
