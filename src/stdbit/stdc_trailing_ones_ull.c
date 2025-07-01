#include <stdbit.h>
#include "cpu.h"

unsigned stdc_trailing_ones_ull(unsigned long long x) __unsequenced
{
    return x == -1ull? 64 : a_ctz_64(~x);
}
