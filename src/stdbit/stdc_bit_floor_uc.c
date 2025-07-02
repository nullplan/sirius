#include <stdbit.h>
#include "cpu.h"

unsigned char stdc_bit_floor_uc(unsigned char x) __unsequenced
{
    return x == 0? 0 : 1 << (8 * sizeof (size_t) - 1 - a_clz(x));
}
