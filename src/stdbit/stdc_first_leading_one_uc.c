#include <stdbit.h>
#include "cpu.h"

unsigned stdc_first_leading_one_uc(unsigned char x) __unsequenced
{
    return x? a_clz(x) - 8 * (sizeof (size_t) - 1) + 1 : 0;
}
