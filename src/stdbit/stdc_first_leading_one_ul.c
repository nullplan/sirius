#include <stdbit.h>
#include "cpu.h"

unsigned stdc_first_leading_one_ul(unsigned long x) __unsequenced
{
    return x? a_clz(x) + 1 : 0;
}
