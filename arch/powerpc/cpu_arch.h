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

static void a_crash(void) {
    __asm__(".long 0" ::: "memory");
    __builtin_unreachable();
}
