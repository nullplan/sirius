#define a_barrier a_barrier
static inline void a_barrier(void)
{
    __asm__("sync" ::: "memory");
}

#define a_pre_llsc a_barrier
#define a_post_llsc a_post_llsc
static inline void a_post_llsc(void)
{
    __asm__("isync" ::: "memory");
}

static inline int a_ll(volatile int *p)
{
    int r;
    __asm__ volatile("lwarx %0, 0, %1" : "=r"(r) : "r"(p), "m"(*p));
    return r;
}

static inline int a_sc(volatile int *p, int v)
{
    int cr;
    __asm__ ("stwcx. %3, 0, %2; mfcr %0" : "=r"(cr), "=m"(*p) : "r"(p), "r"(v) : "cc");
    return cr & 0x20000000;
}

#define a_ll_p a_ll_p
static inline void *a_ll_p(void *volatile *p)
{
    void *r;
    __asm__ volatile("ldarx %0, 0, %1" : "=r"(r) : "r"(p), "m"(*p));
    return r;
}

#define a_sc_p a_sc_p
static inline int a_sc_p(void *volatile *p, void *v)
{
    int cr;
    __asm__ ("stdcx. %3, 0, %2; mfcr %0" : "=r"(cr), "=m"(*p) : "r"(p), "r"(v) : "cc");
    return cr & 0x20000000;
}

static inline void a_crash(void) {
    __asm__(".long 0" ::: "memory");
}

#define a_clz a_clz
#define a_clz_64 a_clz
static inline int a_clz(size_t x) {
    __asm__("cntlzd %0, %1" : "=r"(x) : "r"(x));
    return x;
}

#ifdef _ARCH_PWR9
#define a_ctz a_ctz
static inline int a_ctz(size_t x) {
    __asm__("cnttzd %0, %1" : "=r"(x) : "r"(x));
    return x;
}
#endif

#define a_mul128 a_mul128
static inline struct uint128 a_mul128(uint64_t a, uint64_t b)
{
    struct uint128 res;
    __asm__("mulhdu %0,%1,%2" : "=r"(res.hi) : "r"(a), "r"(b));
    res.lo = a * b;
    return res;
}

static inline _Noreturn void a_stackjmp(void *func, void *stack)
{
    register long r12 __asm__("r12") = (long)func;
    __asm__("mr %%r1, %1; mtctr %0; bctr" :: "r"(r12), "r"(stack));
}

static inline _Noreturn void a_stackinvoke(void (*func)(void), void *stack)
{
    stack = (void *)(((uintptr_t)stack & -16ul) - 32);
    a_stackjmp((void *)func, stack);
}

#ifdef _ARCH_PWR5
#define a_popcnt_32 a_popcnt_32
static inline int a_popcnt_32(uint32_t x)
{
    uint64_t X = x;
    __asm__("popcntb %0, %1" : "=r"(X) : "r"(X));
    return (X * 0x01010101) >> 24;
}

#define a_popcnt_64 a_popcnt_64
static inline int a_popcnt_64(uint64_t x)
{
    __asm__("popcntb %0, %1" : "=r"(x) : "r"(x));
    return (x * 0x0101010101010101) >> 56;
}
#endif
