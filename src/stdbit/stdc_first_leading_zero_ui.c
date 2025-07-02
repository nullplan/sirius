#include <stdbit.h>
#include "cpu.h"

unsigned stdc_first_leading_zero_ui(unsigned x) __unsequenced
{
    return x == 0xffffffff? 0 : a_clz(~((size_t)x << (8 * (sizeof (size_t) - 4)))) + 1;
}
