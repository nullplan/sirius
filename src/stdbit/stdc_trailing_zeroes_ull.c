#include <stdbit.h>
#include "cpu.h"
unsigned stdc_trailing_zeroes_ull(unsigned long long x) __unsequenced
{
    return x? a_ctz_64(x) : 64;
}
