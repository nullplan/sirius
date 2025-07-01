#include <stdbit.h>
#include "cpu.h"

unsigned stdc_leading_zeroes_ui(unsigned x) __unsequenced
{
    return x? a_clz(x) - 8 * (sizeof (size_t) - 4) : 32;
}
