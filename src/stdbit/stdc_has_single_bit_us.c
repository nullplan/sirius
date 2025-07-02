#include <stdbit.h>
bool stdc_has_single_bit_us(unsigned short x) __unsequenced
{
    return x && !(x & (x - 1));
}
