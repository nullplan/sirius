#include <stdbit.h>
#include "cpu.h"

unsigned stdc_trailing_ones_ul(unsigned long x) __unsequenced
{
    return x == -1ul? 8 * sizeof(x) : a_ctz(~x);
}
