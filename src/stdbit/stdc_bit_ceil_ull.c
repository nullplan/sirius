#include <stdbit.h>
#include "cpu.h"

unsigned long long stdc_bit_ceil_ull(unsigned long long x) __unsequenced
{
    return x == 0? 1 : 1ull << (63 - a_clz_64(x)) << 1;
}
