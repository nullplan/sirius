#include <stdbit.h>
#include "cpu.h"

unsigned long stdc_bit_floor_ul(unsigned long x) __unsequenced
{
    return x == 0? 0 : 1ul << (8 * sizeof (size_t) - 1 - a_clz(x));
}
