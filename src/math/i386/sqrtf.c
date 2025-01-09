#include <math.h>

float sqrtf(float x)
{
    __asm__("fsqrt" : "+t"(x) :: "memory");
    return x;
}
