#include <stdbit.h>
#include "cpu.h"

unsigned short stdc_bit_floor_us(unsigned short x) __unsequenced
{
    return x == 0? 0 : 1 << (8 * sizeof (size_t) - 1 - a_clz(x));
}
