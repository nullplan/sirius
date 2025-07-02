#include <stdbit.h>
#include "cpu.h"

unsigned stdc_first_leading_zero_ull(unsigned long long x) __unsequenced
{
    return x == -1ull? 0 : a_clz_64(~x) + 1;
}
