#define a_cas a_cas
static inline int a_cas(volatile int *p, int e, int n)
{
    __asm__("lock; cmpxchgl %2, %1" : "+a"(e), "+m"(*p) : "r"(n) : "cc");
    return e;
}

#define a_cas_p a_cas_p
static inline void *a_cas_p(void *volatile *p, void *e, void *n)
{
    __asm__("lock; cmpxchgq %2, %1" : "+a"(e), "+m"(*p) : "r"(n) : "cc");
    return e;
}

#define a_swap a_swap
static inline int a_swap(volatile int *p, int v)
{
    __asm__("lock; xchgl %1, %0" : "+m"(*p), "+r"(v));
    return v;
}

static inline void a_crash(void) {
    __asm__("ud2" ::: "memory");
    __builtin_unreachable();
}

#define a_ctz a_ctz
static inline int a_ctz(size_t x)
{
    __asm__("bsf %1,%0" : "=r"(x) : "rm"(x) : "cc");
    return x;
}

#define a_mul128 a_mul128
static inline struct uint128 a_mul128(uint64_t a, uint64_t b)
{
    struct uint128 res;
    __asm__("mulq %2" : "=a"(res.lo), "=d"(res.hi) : "r"(b), "a"(a) : "cc");
    return res;
}
