#ifndef CPU_H
#define CPU_H

#include <stddef.h>
#include <stdint.h>
struct uint128 { uint64_t hi, lo; };
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
    return (void *)a_cas((volatile int *)p, (int)e, (int)n);
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

#ifndef a_inc
static inline void a_inc(volatile int *p) {
    int v;
    a_pre_llsc();
    do v = a_ll(p);
    while (!a_sc(p, v + 1u));
    a_post_llsc();
}
#endif

#ifndef a_dec
static inline void a_dec(volatile int *p) {
    int v;
    a_pre_llsc();
    do v = a_ll(p);
    while (!a_sc(p, v - 1u));
    a_post_llsc();
}
#endif

#ifndef a_or_l
#ifndef a_ll_p
#define a_ll_p(x) a_ll((volatile int *)(x))
#define a_sc_p(p, v) a_sc((volatile int *)(p), (int)(v))
#endif
static inline void a_or_l(volatile unsigned long *p, unsigned long m)
{
    unsigned long v;
    a_pre_llsc();
    do v = (unsigned long)a_ll_p((void *volatile *)p);
    while (!a_sc_p((void *volatile *)p, (void *)(v | m)));
    a_post_llsc();
}
#endif
#ifndef a_ctz
#ifdef a_clz
static inline int a_ctz(size_t x) {
    return 8 * sizeof (size_t) - 1 - a_clz(x & -x);
}
#else
#define a_ctz_32 a_ctz_32
static inline int a_ctz_32(uint32_t x) {
    static const unsigned char pos[] = {
        0, 1, 2, 6, 3, 11 ,7, 16, 4, 14, 12, 21, 8, 23, 17, 26,
        31, 5, 10, 15, 13, 20, 22, 25, 30, 9, 19, 24, 29, 18, 28, 27,
    };
    return pos[((x & -x) * 0x04653adf) >> 27];
}

#define a_ctz_64 a_ctz_64
static inline int a_ctz_64(uint64_t x) {
    static const unsigned char pos[] = {
        0,1,2,4,8,17,34,5,11,23,47,31,63,62,61,59,
        55,46,29,58,53,43,22,44,24,49,35,7,15,30,60,57,
        51,38,12,25,50,36,9,18,37,10,21,42,20,41,19,39,
        14,28,56,48,33,3,6,13,27,54,45,26,52,40,16,32
    };
    return pos[((x & -x) * 0x022fdd63cc95386dull) >> 58];
}

static inline int a_ctz(size_t x) {
    if (sizeof (size_t) == 8) return a_ctz_64(x);
    return a_ctz_32(x);
}
#endif
#endif

#ifndef a_ctz_64
static inline int a_ctz_64(uint64_t x) {
    if (sizeof (size_t) == 8) return a_ctz(x);
    if (!(size_t)x) return 32 + a_ctz(x>>32);
    return a_ctz(x);
}
#endif

#ifndef a_clz_64
static inline int a_clz_64(uint64_t x) {
    if (sizeof (size_t) == 8) return a_clz(x);
    if (x >> 32) return a_clz(x >> 32);
    return 32+a_clz(x);
}
#endif

#ifndef a_mul128
static inline struct uint128 a_mul128(uint64_t a, uint64_t b)
{
    /* portable version:
     * Split both inputs into two 32-bit numbers.
     * (a_hi * 2³² + a_lo) (b_hi * 2³² + b_lo)
     * = a_hi b_hi * 2⁶⁴ + (a_hi b_lo + a_lo b_hi) * 2³² + a_lo b_lo
     *
     * (2³²-1)² = 2⁶⁴ - 2³³ + 1
     * 2(2³²-1)² = 2⁶⁵ - 2³⁴ + 2 > 2⁶⁴
     * i.e. the mid calculation can overflow
     */
    uint64_t a_hi = a >> 32;
    uint64_t a_lo = a & 0xffffffff;
    uint64_t b_hi = b >> 32;
    uint64_t b_lo = b & 0xffffffff;
    struct uint128 res;
    res.lo = a_lo * b_lo;
    res.hi = a_hi * b_hi;
    uint64_t mid1 = a_hi * b_lo;
    uint64_t mid = mid1 + a_lo * b_hi;
    res.lo += mid << 32;
    res.hi += (mid >> 32) + (res.lo < (mid << 32)) + ((0ull + (mid < mid1)) << 32);
    return res;
}
#endif

#ifndef a_pause
#define a_pause()
#endif

#ifndef a_popcnt_32
static inline int a_popcnt_32(uint32_t x)
{
    x -= (x >> 1) & 0x55555555;
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x + (x >> 4)) & 0x0f0f0f0f;
    return (x * 0x01010101) >> 24;
}
#endif

#ifndef a_popcnt_64
static inline int a_popcnt_64(uint64_t x)
{
    /* on 32-bit machines, avoid 64-bit math. */
    if (sizeof (size_t) == 4) return a_popcnt_32(x) + a_popcnt_32(x >> 32);

    x -= (x >> 1) & 0x5555555555555555;
    x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333);
    x = (x + (x >> 4)) & 0x0f0f0f0f0f0f0f0f;
    return (x * 0x0101010101010101) >> 56;
}
#endif

#ifndef a_popcnt
static inline int a_popcnt(size_t x)
{
    if (sizeof (size_t) == 4) return a_popcnt_32(x);
    return a_popcnt_64(x);
}
#endif
#endif
