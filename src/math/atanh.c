/* @(#)e_atanh.c 1.3 95/01/18 */
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

/* __ieee754_atanh(x)
 * Method :
 *    1.Reduced x to positive by atanh(-x) = -atanh(x)
 *    2.For x>=0.5
 *                  1              2x                          x
 *	atanh(x) = --- * log(1 + -------) = 0.5 * log1p(2 * --------)
 *                  2             1 - x                      1 - x
 *	
 * 	For x<0.5
 *	atanh(x) = 0.5*log1p(2x+2x*x/(1-x))
 *
 * Special cases:
 *	atanh(x) is NaN if |x| > 1 with signal;
 *	atanh(NaN) is that NaN with no signal;
 *	atanh(+-1) is +-INF with signal.
 *
 */

#include "libm.h"

static const double one = 1.0, huge = 1e300;

#ifdef __GNUC__
#define fabs __builtin_fabs
#endif

double atanh(double x)
{
    double t;
    int hx,ix;
    unsigned lx;
    hx = __double_bits(x) >> 32;		/* high word */
    lx = __double_bits(x);			/* low word */
    ix = hx & 0x7fffffff;
    if ((ix | ((lx | (-lx)) >> 31)) > 0x3ff00000) /* |x|>1 */
        return (x - x) / (x - x);
    if (ix == 0x3ff00000) 
        return x / 0.0;
    if (ix < 0x3e300000) {
        FORCE_EVAL(huge + x);
        return x;	/* x<2**-28 */
    }
    x = fabs(x);
    if(ix < 0x3fe00000) {		/* x < 0.5 */
        t = x + x;
        t = 0.5 * log1p(t + t * x / (one - x));
    } else 
        t = 0.5 * log1p((x + x) / (one - x));
    if (hx >= 0) return t;
    else return -t;
}
