#include <stdint.h>

static inline uint32_t __fpsr(void)
{
    uint64_t r;
    __asm__("mrs %0, fpsr" : "=r"(r) :: "memory");
    return r;
}

static inline void __set_fpsr(uint32_t r)
{
    __asm__("msr fpsr, %0" :: "r"(r) : "memory");
}

static inline uint32_t __fpcr(void)
{
    uint64_t r;
    __asm__("mrs %0, fpcr" : "=r"(r) :: "memory");
    return r;
}

static inline void __set_fpcr(uint32_t r)
{
    __asm__("msr fpcr, %0" :: "r"(r) : "memory");
}

