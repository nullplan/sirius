#include "libc.h"
#include "x86_cpuid.h"
#include <stdint.h>

hidden uintptr_t __cpuid;

struct regs {
    uint32_t ax, bx, cx, dx;
};

static struct regs cpuid(uint32_t fn)
{
    struct regs r;
    __asm__("cpuid" : "=a"(r.ax), "=b"(r.bx), "=c"(r.cx), "=d"(r.dx) : "a"(fn));
    return r;
}

static int cpu_has_fma(void)
{
    struct regs r = cpuid(1);
    return r.cx & 0x1000;
}

static int cpu_has_fma4(void)
{
    struct regs r = cpuid(0x80000001);
    return r.cx & 0x10000;
}

static int cpu_is_amd(void)
{
    struct regs r = cpuid(0);
    return r.bx == 0x68747541 && r.cx == 0x444d4163 && r.dx == 0x69746e65;
}

hidden void __init_cpuid(void)
{
    if (cpu_has_fma())
        __cpuid |= X86_FEAT_FMA;
    if (cpu_is_amd() && cpu_has_fma4())
        __cpuid |= X86_FEAT_FMA4;
}
