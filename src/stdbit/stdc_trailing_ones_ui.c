#include <stdbit.h>
#include "cpu.h"

unsigned stdc_trailing_ones_ui(unsigned x) __unsequenced
{
    return x == -1u? 32 : a_ctz(~(size_t)x);
}
