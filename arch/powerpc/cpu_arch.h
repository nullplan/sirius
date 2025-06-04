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

static inline void a_crash(void) {
    __asm__(".long 0" ::: "memory");
    for (;;);
}

#define a_clz a_clz
static inline int a_clz(size_t x) {
    __asm__("cntlzw %0, %1" : "=r"(x) : "r"(x));
    return x;
}

#ifdef _ARCH_PWR9
#define a_ctz a_ctz
static inline int a_ctz(size_t x) {
    __asm__("cnttzw %0, %1" : "=r"(x) : "r"(x));
    return x;
}
#endif

static inline void a_stackjmp(void *func, void *stack)
{
    __asm__("mr %%r1, %1; mtctr %0; bctr" :: "r"(func), "r"(stack));
    for (;;);
}

static inline void a_stackinvoke(void (*func)(void), void *stack)
{
    stack = (void *)(((uintptr_t)stack & -16ul) - 16);
    a_stackjmp((void *)func, stack);
}
