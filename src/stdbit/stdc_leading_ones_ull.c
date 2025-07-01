#include <stdbit.h>
#include "cpu.h"

unsigned stdc_leading_ones_ull(unsigned long long x) __unsequenced
{
    return x == -1ull? 64 : a_clz_64(~x);
}
