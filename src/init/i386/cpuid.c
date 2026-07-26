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

static uint32_t get_flags(void)
{
    uint32_t val;
    __asm__("pushfl; popl %0" : "=rm"(val) :: "memory");
    return val;
}

static void set_flags(uint32_t val)
{
    __asm__("pushl %0; popfl" :: "g"(val) : "memory");
}

static int cpu_has_cpuid(void)
{
    uint32_t fl = get_flags();
    set_flags(fl ^ 0x200000);
    return (get_flags() ^ fl) & 0x200000;
}

static int cpu_has_cpuid_fn(uint32_t val)
{
    struct regs r = cpuid(0);
    return r.ax >= val;
}

static int cpu_has_cpuid_ext_fn(uint32_t val)
{
    struct regs r = cpuid(0x80000000);
    return r.ax >= val;
}

hidden void __init_cpuid(void)
{
    if (!cpu_has_cpuid())
        return;
    if (cpu_has_cpuid_fn(1) && cpu_has_fma())
        __cpuid |= X86_FEAT_FMA;
    if (cpu_is_amd() && cpu_has_cpuid_ext_fn(0x80000001) && cpu_has_fma4())
        __cpuid |= X86_FEAT_FMA4;
}
