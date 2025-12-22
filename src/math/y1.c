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

static const double
        invsqrtpi=  5.64189583547756279280e-01, /* 0x3FE20DD7, 0x50429B6D */
        tpi      =  6.36619772367581382433e-01; /* 0x3FE45F30, 0x6DC9C883 */

static const double U0[5] = {
    -1.96057090646238940668e-01, /* 0xBFC91866, 0x143CBC8A */
    5.04438716639811282616e-02, /* 0x3FA9D3C7, 0x76292CD1 */
    -1.91256895875763547298e-03, /* 0xBF5F55E5, 0x4844F50F */
    2.35252600561610495928e-05, /* 0x3EF8AB03, 0x8FA6B88E */
    -9.19099158039878874504e-08, /* 0xBE78AC00, 0x569105B8 */
};
static const double V0[5] = {
    1.99167318236649903973e-02, /* 0x3F94650D, 0x3F4DA9F0 */
    2.02552581025135171496e-04, /* 0x3F2A8C89, 0x6C257764 */
    1.35608801097516229404e-06, /* 0x3EB6C05A, 0x894E8CA6 */
    6.22741452364621501295e-09, /* 0x3E3ABF1D, 0x5BA69A86 */
    1.66559246207992079114e-11, /* 0x3DB25039, 0xDACA772A */
};

double y1(double x) 
{
    double z, s,c,ss,cc,u,v;
    int32_t hx;
    uint32_t lx;
    int ix;

    hx = __double_bits(x) >> 32;
    lx = __double_bits(x);
    ix = 0x7fffffff&hx;
    /* if Y1(NaN) is NaN, Y1(-inf) is NaN, Y1(inf) is 0 */
    if(ix>=0x7ff00000) return  1.0/(x+x*x); 
    if((ix|lx)==0) return -1.0/0.0;
    if(hx<0) return 0.0/0.0;
    if(ix >= 0x40000000) {  /* |x| >= 2.0 */
        s = sin(x);
        c = cos(x);
        ss = -s-c;
        cc = s-c;
        if(ix<0x7fe00000) {  /* make sure x+x not overflow */
            z = cos(x+x);
            if ((s*c)>0.0) cc = z/ss;
            else            ss = z/cc;
        }
        /* y1(x) = sqrt(2/(pi*x))*(p1(x)*sin(x0)+q1(x)*cos(x0))
         * where x0 = x-3pi/4
         *      Better formula:
         *              cos(x0) = cos(x)cos(3pi/4)+sin(x)sin(3pi/4)
         *                      =  1/sqrt(2) * (sin(x) - cos(x))
         *              sin(x0) = sin(x)cos(3pi/4)-cos(x)sin(3pi/4)
         *                      = -1/sqrt(2) * (cos(x) + sin(x))
         * To avoid cancellation, use
         *              sin(x) +- cos(x) = -cos(2x)/(sin(x) -+ cos(x))
         * to compute the worse one.
         */
        if(ix>0x48000000) z = (invsqrtpi*ss)/sqrt(x);
        else {
            u = __pone(x); v = __qone(x);
            z = invsqrtpi*(u*ss+v*cc)/sqrt(x);
        }
        return z;
    } 
    if(ix<=0x3c900000) {    /* x < 2**-54 */
        return(-tpi/x);
    } 
    z = x*x;
    u = U0[0]+z*(U0[1]+z*(U0[2]+z*(U0[3]+z*U0[4])));
    v = 1.0+z*(V0[0]+z*(V0[1]+z*(V0[2]+z*(V0[3]+z*V0[4]))));
    return(x*(u/v) + tpi*(j1(x)*log(x)-1.0/x));
}
