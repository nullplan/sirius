#include <stdbit.h>
#include "cpu.h"

unsigned long stdc_bit_ceil_ul(unsigned long x) __unsequenced
{
    return x == 0? 1 : 1ul << (8 * sizeof (size_t) - 1 - a_clz(x)) << 1;
}
