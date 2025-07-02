#include <stdbit.h>
#include "cpu.h"

unsigned stdc_first_trailing_one_ul(unsigned long x) __unsequenced
{
    return x == 0? 0 : a_ctz(x) + 1;
}
