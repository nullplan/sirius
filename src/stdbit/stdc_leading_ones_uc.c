#include <stdbit.h>
#include "cpu.h"

unsigned stdc_leading_ones_uc(unsigned char x) __unsequenced
{
    return x == 0xff? 8 : a_clz(~x) - 8 * (sizeof (size_t) - 1);
}
