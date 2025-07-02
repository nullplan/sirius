#include <stdbit.h>
#include "cpu.h"

unsigned char stdc_bit_ceil_uc(unsigned char x) __unsequenced
{
    return x == 0? 1 : 1 << (8 * sizeof (size_t) - a_clz(x));
}
