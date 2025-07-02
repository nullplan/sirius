#include <stdbit.h>
#include "cpu.h"

unsigned stdc_first_leading_one_ui(unsigned x) __unsequenced
{
    return x? a_clz(x) - 8 * (sizeof (size_t) - 4) + 1 : 0;
}
