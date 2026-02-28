/* @(#)s_cbrt.c 1.3 95/01/18 */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 *
 */

#include "libm.h"

/* cbrt(x)
 * Return cube root of x
 */
static const unsigned 
	B1 = 715094163, /* B1 = (682-0.03306235651)*2**20 */
	B2 = 696219795; /* B2 = (664-0.03306235651)*2**20 */

static const double
C =  5.42857142857142815906e-01, /* 19/35     = 0x3FE15F15, 0xF15F15F1 */
D = -7.05306122448979611050e-01, /* -864/1225 = 0xBFE691DE, 0x2532C834 */
E =  1.41428571428571436819e+00, /* 99/70     = 0x3FF6A0EA, 0x0EA0EA0F */
F =  1.60714285714285720630e+00, /* 45/28     = 0x3FF9B6DB, 0x6DB6DB6E */
G =  3.57142857142857150787e-01; /* 5/14      = 0x3FD6DB6D, 0xB6DB6DB7 */

#ifdef __GNUC__
#define fabs __builtin_fabs
#endif

double cbrt(double x) 
{
    int	hx;
    double r,s,t,w;
    unsigned sign;

    hx = __double_bits(x) >> 32;		/* high word of x */
    sign = hx & 0x80000000; 		/* sign= sign(x) */
    hx ^= sign;
    if (hx >= 0x7ff00000) return x + x; /* cbrt(NaN,INF) is itself */
    if (!(__double_bits(x) << 1))
        return(x);		/* cbrt(0) is itself */

    x = fabs(x);
    /* rough cbrt to 5 bits */
    if (hx < 0x00100000) 		/* subnormal number */
    {
        t = 0x1p54 * x;
        hx = __double_bits(t) >> 32;
        hx = hx / 3 + B2;
        t = __double_from_bits((unsigned)(__double_bits(t)) | ((hx + 0ull) << 32));
    }
    else
    {
        t = __double_from_bits((hx / 3 + B1 + 0ull) << 32);
    }

    /* new cbrt to 23 bits, may be implemented in single precision */
    r = t * t / x;
    s = C + r * t;
    t *= G + F / (s + E + D / s);	

    /* chopped to 20 bits and make it larger than cbrt(x) */ 
    t = __double_from_bits(((__double_bits(t) >> 32) + 1) << 32);

    /* one step newton iteration to 53 bits with error less than 0.667 ulps */
    s = t * t;		/* t*t is exact */
    r = x / s;
    w = t + t;
    r = (r - t) / (w + r);	/* r-s is exact */
    t = t + t * r;

    /* retore the sign bit */
    if (sign) t = -t;
    return(t);
}
