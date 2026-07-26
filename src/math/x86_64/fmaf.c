#include "libm.h"
#include "libc.h"
#include "x86_cpuid.h"

static inline float fmaf_fma(float x, float y, float z)
{
    __asm__("vfmadd132ss %1, %2, %0" : "+x"(x) : "x"(y), "x"(z));
    return x;
}

static inline float fmaf_fma4(float x, float y, float z)
{
    __asm__("vfmaddss %3, %2, %1, %0" : "=x"(x) : "x"(x), "x"(y), "x"(z));
    return x;
}

#if !__FMA__ && !__FMA4__
static float fmaf_soft(float, float, float);
#define fmaf fmaf_soft
#include "../fmaf.c"
#undef fmaf
#endif

float fmaf(float x, float y, float z)
{
#if __FMA__
    return fmaf_fma(x, y, z);
#elif __FMA4__
    return fmaf_fma4(x, y, z);
#else
    if (__cpuid & X86_FEAT_FMA)
        return fmaf_fma(x, y, z);
    if (__cpuid & X86_FEAT_FMA4)
        return fmaf_fma4(x, y, z);
    return fmaf_soft(x, y, z);
#endif
}
