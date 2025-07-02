#include <stdbit.h>
bool stdc_has_single_bit_uc(unsigned char x) __unsequenced
{
    return x && !(x & (x - 1));
}
