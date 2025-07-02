#include <stdbit.h>
bool stdc_has_single_bit_ul(unsigned long x) __unsequenced
{
    return x && !(x & (x - 1));
}
