#include <stdbit.h>
#include "cpu.h"

unsigned stdc_leading_zeroes_ull(unsigned long long x) __unsequenced
{
    return x? a_clz_64(x) : sizeof (x) * 8;
}
