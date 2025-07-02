#include <stdbit.h>
#include "cpu.h"

unsigned stdc_first_trailing_zero_ull(unsigned long long x) __unsequenced
{
    return x == -1ull? 0 : a_ctz_64(~x) + 1;
}
