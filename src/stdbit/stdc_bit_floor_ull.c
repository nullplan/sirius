#include <stdbit.h>
#include "cpu.h"

unsigned long long stdc_bit_floor_ull(unsigned long long x) __unsequenced
{
    return x == 0? 0 : 1ull << (63 - a_clz_64(x));
}
