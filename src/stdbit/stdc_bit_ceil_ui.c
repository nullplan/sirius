#include <stdbit.h>
#include "cpu.h"

unsigned stdc_bit_ceil_ui(unsigned x) __unsequenced
{
    return x == 0? 1 : 1u << (8 * sizeof (size_t) - 1 - a_clz(x)) << 1;
}
