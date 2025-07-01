#include <stdbit.h>
#include "cpu.h"

unsigned int stdc_leading_zeroes_uc(unsigned char x) __unsequenced
{
    return x? a_clz(x) - 8 * (sizeof (size_t) - 1) : 8;
}
