#include <math.h>
#include "libc.h"
#include "x86_cpuid.h"
#pragma GCC target "sse"

static inline double fma_fma(double x, double y, double z)
{
    __asm__("vfmadd132sd %1, %2, %0" : "+x"(x) : "xm"(y), "x"(z));
    return x;
}

static inline double fma_fma4(double x, double y, double z)
{
    __asm__("vfmaddsd %3, %2, %1, %0" : "=x,x,x"(x) : "x,x,x"(x), "x,x,m"(y), "x,m,x"(z));
    return x;
}

#if !__FMA__ && !__FMA4__
static double fma_soft(double x, double y, double z);
#define fma fma_soft
#include "../fma.c"
#undef fma
#endif

double fma(double x, double y, double z)
{
#if __FMA__
    return fma_fma(x, y, z);
#elif __FMA4__
    return fma_fma4(x, y, z);
#else
    if (__cpuid & X86_FEAT_FMA)
        return fma_fma(x, y, z);
    if (__cpuid & X86_FEAT_FMA4)
        return fma_fma4(x, y, z);
    return fma_soft(x, y, z);
#endif
}
