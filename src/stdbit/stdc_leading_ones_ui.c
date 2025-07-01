#include <stdbit.h>
#include "cpu.h"

unsigned stdc_leading_ones_ui(unsigned x) __unsequenced
{
    return x == 0xffffffff? 32 : a_clz(~(size_t)x) - 8 * (sizeof (size_t) - 4);
}
