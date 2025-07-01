#include <stdbit.h>
#include "cpu.h"
unsigned stdc_trailing_zeroes_ui(unsigned x) __unsequenced
{
    return x? a_ctz(x) : 32;
}
