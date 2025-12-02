#include <stddef.h>
extern hidden size_t __a_barrier_ptr;
extern hidden size_t __a_cas_ptr;

#if __ARM_ARCH < 5
#define BLX "mov lr,pc; bx "
#else
#define BLX "blx "
#endif

#define a_barrier a_barrier
static inline void a_barrier(void)
{
    __asm__(BLX "%0" :: "r"(__a_barrier_ptr) : "memory", "lr");
}

#define a_cas a_cas
static inline int a_cas(volatile int *ptr, int ex, int ne)
{
    /* kuser CAS is a boolean CAS. a_cas() is a value CAS. The translation works something like: */
    for (;;)
    {
        /* 1st attempt CAS */
        register int r0 __asm__("r0") = ex;
        register int r1 __asm__("r1") = ne;
        register volatile int *r2 __asm__("r2") = ptr;
        register size_t r3 __asm__("r3") = __a_cas_ptr;
        __asm__(BLX "%0" : "+r"(r3), "+r"(r0), "+m"(*ptr) : "r"(r1), "r"(r2) : "cc", "ip", "lr");
        if (!r0) return ex; /* if it worked, we have to return ex */
        /* else there was something else at *ptr, so read the value now. */
        int old = *ptr;
        /* but it is possible the value was changed between __a_cas_* reading it and us doing it now.
         * In particular, it is possible the value was changed to ex.
         * So in that case, we just retry again.
         * If CAS failed, we must not return ex.
         */
        if (old != ex) return old;
    }
}

/* for old platforms this could be implemented in terms of SWP.
 * However, since the memory model is implementation-defined on these,
 * and no defined memory barrier exists (CP15 being pervasive but not
 * specified in the architecture), I can only go through __a_cas_ptr
 * and hope that the function referenced there does it right.
 */
#define a_swap a_swap
static inline int a_swap(volatile int *ptr, int v)
{
    int old;
    do old = *ptr;
    while (a_cas(ptr, old, v) != old);
    return old;
}

#define a_inc a_inc
static inline void a_inc(volatile int *ptr)
{
    int old;
    do old = *ptr;
    while (a_cas(ptr, old, old + 1u) != old);
}

#define a_dec a_dec
static inline void a_dec(volatile int *ptr)
{
    int old;
    do old = *ptr;
    while (a_cas(ptr, old, old - 1u) != old);
}

#define a_or_l a_or_l
static inline void a_or_l(volatile unsigned long *ptr, unsigned long m)
{
    unsigned long old;
    do old = *ptr;
    while (a_cas((volatile int *)ptr, old, old | m) != old);
}

static inline _Noreturn void a_crash(void)
{
    __asm__("udf 0");
}

/* CLZ: supported for ARMv6 and higher in thumb mode, or ARMv5 in ARM mode */
#if __ARM_ARCH > 5 || (__ARM_ARCH == 5 && !defined __thumb__)
#define a_clz a_clz
static inline int a_clz(size_t x) {
    __asm__("clz %0, %1" : "=r"(x) : "r"(x));
    return x;
}
#endif

/* RBIT: supported on ARMv6 and higher */
#if __ARM_ARCH > 5
#define a_ctz a_ctz
static inline int a_ctz(size_t x) {
    __asm__("rbit %0, %1; clz %0, %0" : "=r"(x) : "r"(x));
    return x;
}
#endif

static inline _Noreturn void a_stackjmp(void *func, void *sp)
{
    __asm__("mov sp, %0; mov ip, %1; bx ip" :: "r"(sp), "r"(func));
}

static inline _Noreturn void a_stackinvoke(void (*func)(void), void *stack)
{
    stack = (void *)((uintptr_t)stack & -8);
    a_stackjmp(func, stack);
}
