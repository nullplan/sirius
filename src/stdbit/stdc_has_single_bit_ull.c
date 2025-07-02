#include <stdbit.h>
bool stdc_has_single_bit_ull(unsigned long long x) __unsequenced
{
    return x && !(x & (x - 1));
}
