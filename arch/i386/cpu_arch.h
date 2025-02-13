#define a_cas a_cas
static int a_cas(volatile int *p, int e, int n)
{
    __asm__("lock; cmpxchgl %2, %1" : "+a"(e), "+m"(*p) : "r"(n) : "cc");
    return e;
}

#define a_swap a_swap
static int a_swap(volatile int *p, int v)
{
    __asm__("lock; xchgl %1, %0" : "+m"(*p), "+r"(v));
    return v;
}

static void a_crash(void)
{
    __asm__("ud2" ::: "memory");
    __builtin_unreachable();
}

#define a_ctz a_ctz
static int a_ctz(size_t x)
{
    __asm__("bsf %1,%0" : "=r"(x) : "rm"(x) : "cc");
    return x;
}

static inline void a_stackinvoke(void (*func)(void), void *stack)
{
    stack = (void *)((uintptr_t)stack & -16ul);
    __asm__("movl %1, %%esp; calll *%0" :: "r"(func), "r"(stack));
    __builtin_unreachable();
}
