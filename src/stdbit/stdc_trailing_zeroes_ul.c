#include <stdbit.h>
#include "cpu.h"
unsigned stdc_trailing_zeroes_ul(unsigned long x) __unsequenced
{
    return x? a_ctz(x) : 8 * sizeof (x);
}
