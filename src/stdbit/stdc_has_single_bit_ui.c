#include <stdbit.h>
bool stdc_has_single_bit_ui(unsigned x) __unsequenced
{
    return x && !(x & (x - 1));
}
