#include <stdbit.h>
#include "cpu.h"

unsigned stdc_leading_ones_ul(unsigned long x) __unsequenced
{
    return x == -1ul? 8 * sizeof (long) : a_clz(~x);
}
