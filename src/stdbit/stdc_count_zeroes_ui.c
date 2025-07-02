#include <stdbit.h>
#include "cpu.h"

unsigned stdc_count_zeroes_ui(unsigned x) __unsequenced
{
    return a_popcnt(~(size_t)x) - 8 * (sizeof (size_t) - 4);
}
