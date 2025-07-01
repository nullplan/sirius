#include <stdbit.h>
#include "cpu.h"

unsigned int stdc_leading_zeroes_us(unsigned short x) __unsequenced
{
    return x? a_clz(x) - 8 * (sizeof (size_t) - 2) : 16;
}
