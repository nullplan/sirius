#include <math.h>
#include <pthread.h>

static double asm_sqrt(double x)
{
    __asm__("fsqrt %0, %1" : "=f"(x) : "f"(x) : "memory");
    return x;
}

#ifndef _ARCH_PPCSQ
static double soft_sqrt(double x);
#define sqrt soft_sqrt
#include "../sqrt.c"
#undef sqrt
#endif

double sqrt(double x)
{
#ifdef _ARCH_PPCSQ
    return asm_sqrt(x);
#else
    if (__pthread_self()->hwcap & 0x00021100)
        return asm_sqrt(x);
    return soft_sqrt(x);
#endif
}
