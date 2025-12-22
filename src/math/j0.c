/* @(#)e_j0.c 1.3 95/01/18 */
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

/* Bessel function of the first kind of order zero.
 * Method -- j0(x):
 *	1. For tiny x, we use j0(x) = 1 - x^2/4 + x^4/64 - ...
 *	2. Reduce x to |x| since j0(x)=j0(-x),  and
 *	   for x in (0,2)
 *		j0(x) = 1-z/4+ z^2*R0/S0,  where z = x*x;
 *	   (precision:  |j0-1+z/4-z^2R0/S0 |<2**-63.67 )
 *	   for x in (2,inf)
 * 		j0(x) = sqrt(2/(pi*x))*(p0(x)*cos(x0)-q0(x)*sin(x0))
 * 	   where x0 = x-pi/4. It is better to compute sin(x0),cos(x0)
 *	   as follow:
 *		cos(x0) = cos(x)cos(pi/4)+sin(x)sin(pi/4)
 *			= 1/sqrt(2) * (cos(x) + sin(x))
 *		sin(x0) = sin(x)cos(pi/4)-cos(x)sin(pi/4)
 *			= 1/sqrt(2) * (sin(x) - cos(x))
 * 	   (To avoid cancellation, use
 *		sin(x) +- cos(x) = -cos(2x)/(sin(x) -+ cos(x))
 * 	    to compute the worse one.)
 *
 *	3 Special cases
 *		j0(nan)= nan
 *		j0(0) = 1
 *		j0(inf) = 0
 */
/* Bessel function of the second kind of order zero.
 * Method -- y0(x):
 *	1. For x<2.
 *	   Since
 *		y0(x) = 2/pi*(j0(x)*(ln(x/2)+Euler) + x^2/4 - ...)
 *	   therefore y0(x)-2/pi*j0(x)*ln(x) is an even function.
 *	   We use the following function to approximate y0,
 *		y0(x) = U(z)/V(z) + (2/pi)*(j0(x)*ln(x)), z= x^2
 *	   where
 *		U(z) = u00 + u01*z + ... + u06*z^6
 *		V(z) = 1  + v01*z + ... + v04*z^4
 *	   with absolute approximation error bounded by 2**-72.
 *	   Note: For tiny x, U/V = u0 and j0(x)~1, hence
 *		y0(tiny) = u0 + (2/pi)*ln(tiny), (choose tiny<2**-27)
 *	2. For x>=2.
 * 		y0(x) = sqrt(2/(pi*x))*(p0(x)*cos(x0)+q0(x)*sin(x0))
 * 	   where x0 = x-pi/4. It is better to compute sin(x0),cos(x0)
 *	   by the method mentioned above.
 *	3. Special cases: y0(0)=-inf, y0(x<0)=NaN, y0(inf)=0.
 */

#include "libm.h"
#pragma STDC FENV_ACCESS ON

static const double
huge 	= 1e300,
invsqrtpi=  5.64189583547756279280e-01, /* 0x3FE20DD7, 0x50429B6D */
 		/* R0/S0 on [0, 2.00] */
R02  =  1.56249999999999947958e-02, /* 0x3F8FFFFF, 0xFFFFFFFD */
R03  = -1.89979294238854721751e-04, /* 0xBF28E6A5, 0xB61AC6E9 */
R04  =  1.82954049532700665670e-06, /* 0x3EBEB1D1, 0x0C503919 */
R05  = -4.61832688532103189199e-09, /* 0xBE33D5E7, 0x73D63FCE */
S01  =  1.56191029464890010492e-02, /* 0x3F8FFCE8, 0x82C8C2A4 */
S02  =  1.16926784663337450260e-04, /* 0x3F1EA6D2, 0xDD57DBF4 */
S03  =  5.13546550207318111446e-07, /* 0x3EA13B54, 0xCE84D5A9 */
S04  =  1.16614003333790000205e-09; /* 0x3E1408BC, 0xF4745D8F */

double j0(double x)
{
    double z, s,c,ss,cc,r,u,v;
    uint32_t ix;

    ix = __double_bits(x) << 1 >> 33;
    if(ix>=0x7ff00000) return 1.0/(x*x);
    x = fabs(x);
    if(ix >= 0x40000000) {	/* |x| >= 2.0 */
        s = sin(x);
        c = cos(x);
        ss = s-c;
        cc = s+c;
        if(ix<0x7fe00000) {  /* make sure x+x not overflow */
            z = -cos(x+x);
            if ((s*c)<0.0) cc = z/ss;
            else 	    ss = z/cc;
        }
        /*
         * j0(x) = 1/sqrt(pi) * (P(0,x)*cc - Q(0,x)*ss) / sqrt(x)
         * y0(x) = 1/sqrt(pi) * (P(0,x)*ss + Q(0,x)*cc) / sqrt(x)
         */
        if(ix>0x48000000) z = (invsqrtpi*cc)/sqrt(x);
        else {
            u = __pzero(x); v = __qzero(x);
            z = invsqrtpi*(u*cc-v*ss)/sqrt(x);
        }
        return z;
    }
    if(ix<0x3f200000) {	/* |x| < 2**-13 */
        if(huge+x>1.0) {	/* raise inexact if x != 0 */
            if(ix<0x3e400000) return 1.0;	/* |x|<2**-27 */
            else 	      return 1.0 - 0.25*x*x;
        }
    }
    z = x*x;
    r =  z*(R02+z*(R03+z*(R04+z*R05)));
    s =  1.0+z*(S01+z*(S02+z*(S03+z*S04)));
    if(ix < 0x3FF00000) {	/* |x| < 1.00 */
        return 1.0 + z*(-0.25+(r/s));
    } else {
        u = 0.5*x;
        return((1.0+u)*(1.0-u)+z*(r/s));
    }
}
