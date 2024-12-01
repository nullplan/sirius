#ifndef __COMPLEX_H
#define __COMPLEX_H

#define complex _Complex
#define _Complex_I  (__extension__(0.f+1.fi))
#define I _Complex_I


complex double   csin(complex double);
complex double   ccos(complex double);
complex double   ctan(complex double);
complex double  casin(complex double);
complex double  cacos(complex double);
complex double  catan(complex double);
complex double  csinh(complex double);
complex double  ccosh(complex double);
complex double  ctanh(complex double);
complex double casinh(complex double);
complex double cacosh(complex double);
complex double catanh(complex double);
complex double   cexp(complex double);
complex double   clog(complex double);
complex double   cpow(complex double, complex double);
complex double  csqrt(complex double);
complex double   conj(complex double);
complex double  cproj(complex double);
        double   carg(        double);
        double   cabs(        double);
        double  creal(        double);
        double  cimag(        double);

complex float   csinf(complex float);
complex float   ccosf(complex float);
complex float   ctanf(complex float);
complex float  casinf(complex float);
complex float  cacosf(complex float);
complex float  catanf(complex float);
complex float  csinhf(complex float);
complex float  ccoshf(complex float);
complex float  ctanhf(complex float);
complex float casinhf(complex float);
complex float cacoshf(complex float);
complex float catanhf(complex float);
complex float   cexpf(complex float);
complex float   clogf(complex float);
complex float   cpowf(complex float, complex float);
complex float  csqrtf(complex float);
complex float   conjf(complex float);
complex float  cprojf(complex float);
        float   cargf(        float);
        float   cabsf(        float);
        float  crealf(        float);
        float  cimagf(        float);

complex long double   csinl(complex long double);
complex long double   ccosl(complex long double);
complex long double   ctanl(complex long double);
complex long double  casinl(complex long double);
complex long double  cacosl(complex long double);
complex long double  catanl(complex long double);
complex long double  csinhl(complex long double);
complex long double  ccoshl(complex long double);
complex long double  ctanhl(complex long double);
complex long double casinhl(complex long double);
complex long double cacoshl(complex long double);
complex long double catanhl(complex long double);
complex long double   cexpl(complex long double);
complex long double   clogl(complex long double);
complex long double   cpowl(complex long double, complex long double);
complex long double  csqrtl(complex long double);
complex long double   conjl(complex long double);
complex long double  cprojl(complex long double);
        long double   cargl(        long double);
        long double   cabsl(        long double);
        long double  creall(        long double);
        long double  cimagl(        long double);


#define creal(x) ((double)(x))
#define crealf(x) ((float)(x))
#define creall(x) ((long double)(x))
#define __cimag(x,t) ((union { complex t __z; t __xy[2]; }){(x)}.__xy[1])
#define cimag(x) __cimag(x, double)
#define cimagf(x) __cimag(x, float)
#define cimagl(x) __cimag(x, long double)

#endif
