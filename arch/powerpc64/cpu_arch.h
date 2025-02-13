#define a_barrier a_barrier
static void a_barrier(void)
{
    __asm__("sync" ::: "memory");
}

#define a_pre_llsc a_barrier
#define a_post_llsc a_post_llsc
static void a_post_llsc(void)
{
    __asm__("isync" ::: "memory");
}

static int a_ll(volatile int *p)
{
    int r;
    __asm__ volatile("lwarx %0, 0, %1" : "=r"(r) : "r"(p), "m"(*p));
    return r;
}

static int a_sc(volatile int *p, int v)
{
    int cr;
    __asm__ ("stwcx. %3, 0, %2; mfcr %0" : "=r"(cr), "=m"(*p) : "r"(p), "r"(v) : "cc");
    return cr & 0x20000000;
}

#define a_ll_p a_ll_p
static void *a_ll_p(void *volatile *p)
{
    void *r;
    __asm__ volatile("ldarx %0, 0, %1" : "=r"(r) : "r"(p), "m"(*p));
    return r;
}

#define a_sc_p a_sc_p
static int a_sc_p(void *volatile *p, void *v)
{
    int cr;
    __asm__ ("stdcx. %3, 0, %2; mfcr %0" : "=r"(cr), "=m"(*p) : "r"(p), "r"(v) : "cc");
    return cr & 0x20000000;
}

static void a_crash(void) {
    __asm__(".long 0" ::: "memory");
    __builtin_unreachable();
}

#define a_clz a_clz
static int a_clz(size_t x) {
    __asm__("cntlzd %0, %1" : "=r"(x) : "r"(x));
    return x;
}

#ifdef _ARCH_PWR9
#define a_ctz a_ctz
static int a_ctz(size_t x) {
    __asm__("cnttzd %0, %1" : "=r"(x) : "r"(x));
    return x;
}

#define a_mul128 a_mul128
static inline struct uint128 a_mul128(uint64_t a, uint64_t b)
{
    struct uint128 res;
    __asm__("mulhdu %0,%1,%2" : "=r"(res.hi) : "r"(a), "r"(b));
    res.lo = a * b;
    return res;
}
#endif

static inline void a_stackinvoke(void (*func)(void), void *stack)
{
    stack = (void *)(((uintptr_t)stack & -16ul) - 32);
    __asm__("mr %%r1, %1; mtctr %%0; bctr" :: "r"(func), "r"(stack));
    __builtin_unreachable();
}
