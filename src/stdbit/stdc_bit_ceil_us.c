#include <stdbit.h>
#include "cpu.h"

unsigned short stdc_bit_ceil_us(unsigned short x) __unsequenced
{
    return x == 0? 1 : 1 << (8 * sizeof (size_t) - a_clz(x));
}
