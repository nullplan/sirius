#include <stdbit.h>
#include "cpu.h"

unsigned stdc_bit_width_uc(unsigned char x) __unsequenced
{
    return x == 0? 0 : 8 * sizeof(size_t) - a_clz(x);
}
