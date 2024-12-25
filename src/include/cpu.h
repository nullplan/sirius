#ifndef CPU_H
#define CPU_H

#include "cpu_arch.h"

#ifndef a_barrier
static inline void a_barrier(void)
{
    __asm__("" ::: "memory");
}
#endif

#ifndef a_pre_llsc
#define a_pre_llsc a_barrier
#endif
#ifndef a_post_llsc
#define a_post_llsc a_barrier
#endif

#ifndef a_cas
static inline int a_cas(volatile int *p, int e, int n)
{
    int v;
    a_pre_llsc();
    do {
        v = a_ll(p);
        if (v != e) break;
    } while (!a_sc(p, n));
    a_post_llsc();
    return v;
}
#endif

#ifndef a_cas_p
#ifdef a_ll_p
static inline void *a_cas_p(void *volatile *p, void *e, void *n)
{
    void *v;
    a_pre_llsc();
    do {
        v = a_ll_p(p);
        if (v != e) break;
    } while (!a_sc_p(p, n));
    a_post_llsc();
    return v;
}
#else
_Static_assert(sizeof (int) == sizeof (void *), "");
static inline void *a_cas_p(void *volatile *p, void *e, void *n)
{
    return (void *)a_cas((volatine int *)p, (int)e, (int)n);
}
#endif
#endif

#ifndef a_swap
static inline int a_swap(volatile int *p, int v)
{
    int r;
    a_pre_llsc();
    do r = a_ll(p);
    while (!a_sc(p, v));
    a_post_llsc();
    return r;
}
#endif
#endif
