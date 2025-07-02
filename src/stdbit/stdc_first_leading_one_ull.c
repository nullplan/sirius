#include <stdbit.h>
#include "cpu.h"

unsigned stdc_first_leading_one_ull(unsigned long long x) __unsequenced
{
    return x? a_clz_64(x) + 1 : 0;
}
