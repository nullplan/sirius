
/* @(#)s_logb.c 1.3 95/01/18 */
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

/*
 * double logb(x)
 * IEEE 754 logb. Included to pass IEEE test suite. Not recommend.
 * Use ilogb instead.
 */

#include "libm.h"

#ifdef __GNUC__
#define fabs __builtin_fabs
#endif

double logb(double x)
{
    int lx,ix;
    ix = __double_bits(x) << 1 >> 33;	/* high |x| */
    lx = __double_bits(x);			/* low x */
    if ((ix | lx) == 0) return -1.0 / fabs(x);
    if (ix >= 0x7ff00000) return x * x;
    if ((ix >>= 20) == 0) 			/* IEEE 754 logb */
        return -1022.0; 
    else
        return (double) (ix-1023); 
}
