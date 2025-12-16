/* @(#)s_floor.c 1.3 95/01/18 */
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
 * floor(x)
 * Return x rounded toward -inf to integral value
 * Method:
 *	Bit twiddling.
 * Exception:
 *	Inexact flag raised if x not equal to floor(x).
 */

#include "libm.h"

static const double huge = 1.0e300;

double floor(double x)
{
    int64_t ix = __double_bits(x);
    int j0;
    j0 = (ix << 1ULL >> 53) - 0x3ff;
    if (j0 >= 52) return x; /* no bits of x are fractional. */
    if (j0 < 0) {  /* ALL bits of x are fractional */
        FORCE_EVAL(huge+x);
        if (ix >= 0) return 0;
        return -1.0;
    }
    /* some bits of x are fractional */
    int64_t mask = ((1ull << 52) - 1) >> j0;
    if (!(ix & mask)) return x; /* x was integral to begin with */
    FORCE_EVAL(huge+x);
    if (ix < 0) ix += mask;
    ix &= ~mask;
    return __double_from_bits(ix);
}
