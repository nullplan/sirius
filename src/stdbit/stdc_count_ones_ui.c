#include <stdbit.h>
#include "cpu.h"

unsigned stdc_count_ones_ui(unsigned x) __unsequenced
{
    return a_popcnt(x);
}
