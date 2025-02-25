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

#define a_inc a_inc
static inline void a_inc(volatile int *p)
{
    __asm__("lock; incl %0" : "+m"(*p) :: "cc");
}

#define a_dec a_dec
static inline void a_dec(volatile int *p)
{
    __asm__("lock; decl %0" : "+m"(*p) :: "cc");
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

#define a_pause a_pause
static inline void a_pause(void)
{
    __asm__("pause" ::: "memory");
}

#define a_or_l a_or_l
static inline void a_or_l(volatile unsigned long *p, unsigned long m)
{
    __asm__("lock; orl %1, %0" : "+m"(*p) : "r"(m) : "cc");
}
