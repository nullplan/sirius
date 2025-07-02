#include <stdbit.h>
#include "cpu.h"

unsigned stdc_first_leading_zero_uc(unsigned char x) __unsequenced
{
    /* well, it was either a conditional or a mod 9, so I chose the conditional */
    return x == 0xff? 0 : a_clz(~((size_t)x << (8 * (sizeof (size_t) - 1)))) + 1;
}
