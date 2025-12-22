#include "libm.h"
#pragma STDC FENV_ACCESS ON
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

static const double invsqrtpi=  5.64189583547756279280e-01; /* 0x3FE20DD7, 0x50429B6D */

double yn(int n, double x)
{
    int i,ix;
    int sign;
    double a, b, temp;
    int32_t hx;
    uint32_t lx;

    hx = __double_bits(x) >> 32;
    lx = __double_bits(x);
    ix = 0x7fffffff&hx;
    /* if Y(n,NaN) is NaN */
    if (isnan(x)) return x+x;
    if((ix|lx)==0) return -1.0/0.0;
    if(hx<0) return 0.0/0.0;
    sign = 1;
    if(n<0){
        n = -n;
        sign = 1 - ((n&1)<<1);
    }
    if(n==0) return(y0(x));
    if(n==1) return(sign*y1(x));
    if(ix==0x7ff00000) return 0.0;
    if(ix>=0x52D00000) { /* x > 2**302 */
        /* (x >> n**2)
         *	    Jn(x) = cos(x-(2n+1)*pi/4)*sqrt(2/x*pi)
         *	    Yn(x) = sin(x-(2n+1)*pi/4)*sqrt(2/x*pi)
         *	    Let s=sin(x), c=cos(x),
         *		xn=x-(2n+1)*pi/4, sqt2 = sqrt(2),then
         *
         *		   n	sin(xn)*sqt2	cos(xn)*sqt2
         *		----------------------------------
         *		   0	 s-c		 c+s
         *		   1	-s-c 		-c+s
         *		   2	-s+c		-c-s
         *		   3	 s+c		 c-s
         */
        switch(n&3) {
            case 0: temp =  sin(x)-cos(x); break;
            case 1: temp = -sin(x)-cos(x); break;
            case 2: temp = -sin(x)+cos(x); break;
            case 3: temp =  sin(x)+cos(x); break;
        }
        b = invsqrtpi*temp/sqrt(x);
    } else {
        a = y0(x);
        b = y1(x);
        /* quit if b is -inf */
        for(i=1;i<n&&(__double_bits(b) != 0xfffULL << 52);i++){
            temp = b;
            b = ((double)(i+i)/x)*b - a;
            a = temp;
        }
    }
    if(sign>0) return b; else return -b;
}
