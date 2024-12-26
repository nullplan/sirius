#define a_barrier a_barrier
static void a_barrier(void)
{
    __asm__("dmb ish" ::: "memory");
}

static inline int a_ll(volatile int *p)
{
    int r;
    __asm__ volatile("ldaxr %0, %1" : "=r"(r) : "m"(p));
    return r;
}

static inline int a_sc(volatile int *p, int v)
{
    int r;
    __asm__("stlxr %0,%2,%1" : "=r"(r), "=Q"(*p) : "r"(v));
    return r;
}

static inline void *a_ll_p(void *volatile *p)
{
    void *r;
    __asm__ volatile("ldaxr %0, %1" : "=r"(r) : "m"(p));
    return r;
}

static inline int a_sc_p(void *volatile *p, void *v)
{
    int r;
    __asm__("stlxr %0,%2,%1" : "=r"(r), "=Q"(*p) : "r"(v));
    return r;
}

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

static void a_crash(void) {
    __asm__("ud" ::: "memory");
    __builtin_unreachable();
}

#define a_ctz a_ctz
static int a_ctz(size_t x)
{
    __asm__("rbit %0, %1; clz %0, %0" : "=r"(x) : "r"(x));
    return x;
}
