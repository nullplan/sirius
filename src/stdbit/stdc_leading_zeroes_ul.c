#include <stdbit.h>
#include "cpu.h"

unsigned stdc_leading_zeroes_ul(unsigned long x) __unsequenced
{
    return x? a_clz(x) : sizeof (x) * 8;
}
