/* @(#)e_remainder.c 1.3 95/01/18 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

/* __ieee754_remainder(x,p)
 * Return :                  
 * 	returns  x REM p  =  x - [x/p]*p as if in infinite 
 * 	precise arithmetic, where [x/p] is the (infinite bit) 
 *	integer nearest x/p (in half way case choose the even one).
 * Method : 
 *	Based on fmod() return x-[x/p]chopped*p exactlp.
 */

#include "libm.h"

static const double zero = 0.0;

#ifdef __GNUC__
#define fmod __builtin_fmod
#define fabs __builtin_fabs
#endif

double remainder(double x, double p)
{
    unsigned sx;
    double p_half;
    uint64_t ix, ip;

    ix = __double_bits(x);
    ip = __double_bits(p);
 
    sx = ix >> 63;
    ix = ix << 1 >> 1;
    ip = ip << 1 >> 1;

    /* purge off exception values */
    if (!ip                             /* p == 0 */
            || ix >= 0x7ffULL << 52     /* x is not finite */
            || ip > 0x7ffULL << 52)     /* p is NaN */
        return (x * p) / (x * p);

    if (ip < 0x7feULL << 52) x = fmod(x, p + p);	/* now x < 2p */
    if (ix == ip) return zero * x;
    x  = fabs(x);
    p  = fabs(p);
    if (ip < 2ULL << 52) {
        if (x + x > p) {
            x -= p;
            if (x + x >= p)
                x -= p;
        }
    } else {
        p_half = 0.5 * p;
        if (x > p_half) {
            x -= p;
            if (x >= p_half)
                x -= p;
        }
    }
    if (sx)
        x = -x;
    return x;
}
