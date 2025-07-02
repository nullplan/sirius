#include <stdbit.h>
#include "cpu.h"

unsigned stdc_first_leading_zero_ul(unsigned long x) __unsequenced
{
    return x == -1ul? 0 : a_clz(~x) + 1;
}
