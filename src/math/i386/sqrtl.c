#include <math.h>

long double sqrtl(long double x)
{
    __asm__("fsqrt" : "+t"(x) :: "memory");
    return x;
}
