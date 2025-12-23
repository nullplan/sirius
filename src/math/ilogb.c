
/* @(#)s_ilogb.c 1.3 95/01/18 */
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

/* ilogb(double x)
 * return the binary exponent of non-zero x
 * ilogb(0) = FP_ILOGB0
 * ilogb(inf/NaN) = FP_ILOGBNAN (no signal is raised)
 */

#include "libm.h"
#include "cpu.h"

int ilogb(double x)
{
    uint64_t ix = __double_bits(x);

    if (ix < 1ull << 52) {
        if (!(ix << 1))
            return FP_ILOGB0;
        else			/* subnormal x */
            return -1010 - a_clz_64(ix);
    }
    else if (ix < 0x7ffULL << 52) return (ix << 1 >> 53) - 0x3ff;
    else return FP_ILOGBNAN;
}
