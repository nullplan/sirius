#include <stdint.h>
#define DEFAULT_X87_CW  0x33f
#define DEFAULT_X87_TW  0xffff
#define DEFAULT_MXCSR   0x1f80
#define HWCAP_SSE       0x02000000

static inline uint16_t __sw(void) {
    uint16_t r;
    __asm__("fnstsw %0" : "=am"(r) :: "memory");
    return r;
}

static inline uint16_t __cw(void)
{
    uint16_t r;
    __asm__("fnstcw %0" : "=m"(r) :: "memory");
    return r;
}

static inline void __set_cw(uint16_t w)
{
    __asm__("fldcw %0" :: "m"(w) : "memory");
}

static inline void __clex(void)
{
    __asm__("fnclex" ::: "memory");
}

struct x87_fenv {
    uint32_t cw;
    uint32_t sw;
    uint32_t tw;
    uint32_t fip;
    uint32_t fcs;
    uint32_t fop;
    uint32_t fds;
};

static inline struct x87_fenv __env(void)
{
    struct x87_fenv env;
    __asm__("fnstenv %0" : "=m"(env) :: "memory");
    return env;
}

static inline void __set_env(const struct x87_fenv *e)
{
    __asm__("fldenv %0" :: "m"(*e) : "memory");
}

static inline uint32_t __mxcsr(void)
{
    uint32_t mxcsr;
    __asm__("stmxcsr %0" : "=m"(mxcsr) :: "memory");
    return mxcsr;
}

static inline void __set_mxcsr(uint32_t mxcsr)
{
    __asm__("ldmxcsr %0" :: "m"(mxcsr) : "memory");
}
