#ifdef __NO_FPRS__
#include "../sqrtf.c"
#else
#include <math.h>
#include <pthread.h>
/* According to GCC, fsqrt is supported on the Cell and on Power4 and all newer ones. */
#define PPCSQ_MASK      		0x000f1100

static float asm_sqrtf(float x)
{
    __asm__("fsqrts %0, %1" : "=f"(x) : "f"(x) : "memory");
    return x;
}

#ifndef _ARCH_PPCSQ
static float soft_sqrtf(float x);
#define sqrtf soft_sqrtf
#include "../sqrtf.c"
#undef sqrtf
#endif

float sqrtf(float x)
{
#ifdef _ARCH_PPCSQ
    return asm_sqrtf(x);
#else
    if (__pthread_self()->hwcap & PPCSQ_MASK)
        return asm_sqrtf(x);
    return soft_sqrtf(x);
#endif
}
#endif
