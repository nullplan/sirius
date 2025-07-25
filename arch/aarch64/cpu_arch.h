#define a_barrier a_barrier
static void a_barrier(void)
{
    __asm__("dmb ish" ::: "memory");
}

static inline int a_ll(volatile int *p)
{
    int r;
    __asm__ volatile("ldaxr %0, [%1]" : "=r"(r) : "r"(p), "m"(*p));
    return r;
}

static inline int a_sc(volatile int *p, int v)
{
    int r;
    __asm__("stlxr %w0,%2,%1" : "=&r"(r), "=Q"(*p) : "r"(v));
    return r;
}

static inline void *a_ll_p(void *volatile *p)
{
    void *r;
    __asm__ volatile("ldaxr %0, [%1]" : "=r"(r) : "r"(p), "m"(*p));
    return r;
}

static inline int a_sc_p(void *volatile *p, void *v)
{
    int r;
    __asm__("stlxr %w0,%2,%1" : "=&r"(r), "=Q"(*p) : "r"(v));
    return r;
}

#define a_pre_llsc()
#define a_post_llsc()

#define a_cas a_cas
static inline int a_cas(volatile int *p, int e, int n)
{
    int v;
    do {
        v = a_ll(p);
        if (v != e) {
            a_barrier();
            break;
        }
    } while (!a_sc(p, n));
    return v;
}

#define a_cas_p a_cas_p
static inline void *a_cas_p(void *volatile *p, void *e, void *n)
{
    void *v;
    do {
        v = a_ll_p(p);
        if (v != e) {
            a_barrier();
            break;
        }
    } while (!a_sc_p(p, n));
    return v;
}

static inline void a_crash(void) {
    __asm__("udf 0" ::: "memory");
    for (;;);
}

#define a_ctz a_ctz
static inline int a_ctz(size_t x)
{
    __asm__("rbit %0, %1; clz %0, %0" : "=r"(x) : "r"(x));
    return x;
}

#define a_clz a_clz_64
#define a_clz_64 a_clz_64
static inline int a_clz_64(uint64_t x)
{
    __asm__("clz %0, %1" : "=r"(x) : "r"(x));
    return x;
}

#define a_mul128 a_mul128
static inline struct uint128 a_mul128(uint64_t a, uint64_t b)
{
    struct uint128 res;
    __asm__("umulh %0,%1,%2" : "=r"(res.hi) : "r"(a), "r"(b));
    res.lo = a * b;
    return res;
}

static inline void a_stackjmp(void *func, void *stack)
{
    __asm__("mov sp, %1; br %0" :: "r"(func), "r"(stack));
    for (;;);
}

static inline void a_stackinvoke(void (*func)(void), void *stack)
{
    stack = (void *)((uintptr_t)stack & -16ul);
    a_stackjmp((void *)func, stack);
}
