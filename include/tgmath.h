#ifndef __TGMATH_H
#define __TGMATH_H

#define __STDC_VERSION_TGMATH_H__ 202311L

/* no C++ this time; C++ does this with overloads in <cmath> */
#include <math.h>
#include <complex.h>

#undef acos
#undef asin
#undef atan
#undef acosh
#undef asinh
#undef atanh
#undef cos
#undef sin
#undef tan
#undef cosh
#undef sinh
#undef tanh
#undef exp
#undef log
#undef sqrt
#undef pow
#undef fabs
#undef atan2
#undef cbrt
#undef ceil
#undef copysign
#undef erf
#undef erfc
#undef exp2
#undef expm1
#undef fdim
#undef floor
#undef fma
#undef fmax
#undef fmin
#undef fmod
#undef frexp
#undef hypot
#undef ilogb
#undef ldexp
#undef lgamma
#undef llrint
#undef llround
#undef log10
#undef log1p
#undef log2
#undef logb
#undef lrint
#undef lround
#undef nearbyint
#undef nextafter
#undef nexttoward
#undef remainder
#undef remquo
#undef rint
#undef round
#undef scalbn
#undef scalbln
#undef tgamma
#undef trunc
#undef carg
#undef creal
#undef cimag
#undef conj
#undef cproj
#undef asinpi
#undef acospi
#undef atanpi
#undef atan2pi
#undef sinpi
#undef cospi
#undef tanpi
#undef exp10
#undef exp10m1
#undef log10p1
#undef log2p1
#undef compoundn
#undef pown
#undef powr
#undef rootn
#undef rsqrt
#undef roundeven
#undef fromfp
#undef ufromfp
#undef fromfpx
#undef ufromfpx
#undef nextup
#undef nextdown
#undef fmaximum
#undef fminimum
#undef llogb
#undef fmaximum_mag
#undef fmaximum_num
#undef fmaximum_mag_num
#undef fminimum_mag
#undef fminimum_num
#undef fminimum_mag_num
#undef fadd
#undef fsub
#undef fmul
#undef fdiv
#undef ffma
#undef fsqrt
#undef dadd
#undef dsub
#undef dmul
#undef ddiv
#undef dfma
#undef dsqrt

#define __tg_sel_cr_1(stem, x) _Generic((x), complex long double: c##stem##l, complex double: c##stem, complex float: c##stem##f, long double: stem##l, float: stem##f, default: stem)
#define acos(x)     __tg_sel_cr_1(acos, x)(x)
#define asin(x)     __tg_sel_cr_1(asin, x)(x)
#define atan(x)     __tg_sel_cr_1(atan, x)(x)
#define acosh(x)    __tg_sel_cr_1(acosh, x)(x)
#define asinh(x)    __tg_sel_cr_1(asinh, x)(x)
#define atanh(x)    __tg_sel_cr_1(atanh, x)(x)
#define cos(x)      __tg_sel_cr_1(cos, x)(x)
#define sin(x)      __tg_sel_cr_1(sin, x)(x)
#define tan(x)      __tg_sel_cr_1(tan, x)(x)
#define cosh(x)     __tg_sel_cr_1(cosh, x)(x)
#define sinh(x)     __tg_sel_cr_1(sinh, x)(x)
#define tanh(x)     __tg_sel_cr_1(tanh, x)(x)
#define exp(x)      __tg_sel_cr_1(exp, x)(x)
#define log(x)      __tg_sel_cr_1(log, x)(x)
#define sqrt(x)     __tg_sel_cr_1(sqrt, x)(x)
#define pow(x, y)   _Generic((x), complex long double: cpowl, \
        complex double: _Generic((y), complex long double: cpowl, long double: cpowl, default: cpow), \
        complex float: _Generic((y), complex long double: cpowl, long double: cpowl, complex float: cpowf, float: cpowf, default: cpow), \
        long double: _Generic((y), complex long double: cpowl, complex double: cpowl, complex float: cpowl, default: powl), \
        float: _Generic((y), complex long double: cpowl, complex double: cpow, complex float: cpowf, long double: powl, float: powf, default: pow), \
        default: _Generic((y), complex long double: cpowl, complex double: cpow, complex float: cpow, long double: powl, default: pow))((x), (y))
#define fabs(x) _Generic((x), complex long double: cabsl, complex double: cabs, complex float: cabsf, long double: fabsl, float: fabsf, default: fabs)(x)

#define __tg_sel_r_1(stem, x) _Generic((x), long double: stem##l, float: stem##f, default: stem)
#define __tg_sel_r_2(stem, x, y) _Generic((x), long double: stem##l, \
        float: _Generic((y), long double: stem##l, float: stem##f, default: stem), \
        default: _Generic((y), long double: stem##l, default: stem))
#define __tg_sel_r_3(stem, x, y, z) _Generic((x), long double: stem##l, \
        float: _Generic((y), long double: stem##l, \
                   float: _Generic((z), long double: stem##l, float: stem##f, default: stem), \
                   default: _Generic(((z), long double: stem##l, default: stem))), \
        default: _Generic((y), long double: stem##l, \
                     default: _Generic((z), long double: stem##l, default: stem)))

#define atan2(x,y)      __tg_sel_r_2(atan2, x, y)((x), (y))
#define cbrt(x)         __tg_sel_r_1(cbrt, x)(x)
#define ceil(x)         __tg_sel_r_1(ceil, x)(x)
#define copysign(x,y)   __tg_sel_r_2(copysign, x, y)((x), (y))
#define erf(x)          __tg_sel_r_1(erf, x)(x)
#define erfc(x)         __tg_sel_r_1(erfc, x)(x)
#define exp2(x)         __tg_sel_r_1(exp2, x)(x)
#define expm1(x)        __tg_sel_r_1(expm1, x)(x)
#define fdim(x, y)      __tg_sel_r_2(fdim, x, y)((x), (y))
#define floor(x)        __tg_sel_r_1(floor, x)(x)
#define fma(x, y, z)    __tg_sel_r_3(fma, x, y, z)((x), (y), (z))
#define fmax(x, y)      __tg_sel_r_2(fmax, x, y)((x), (y))
#define fmin(x, y)      __tg_sel_r_2(fmin, x, y)((x), (y))
#define fmod(x, y)      __tg_sel_r_2(fmod, x, y)((x), (y))
#define frexp(x, y)     __tg_sel_r_1(frexp, x)((x), (y))
#define hypot(x, y)     __tg_sel_r_2(hypot, x, y)((x), (y))
#define ilogb(x)        __tg_sel_r_1(ilogb, x)(x)
#define ldexp(x, y)     __tg_sel_r_1(ldexp, x)((x), (y))
#define lgamma(x)       __tg_sel_r_1(lgamma, x)(x)
#define llrint(x)       __tg_sel_r_1(llrint, x)(x)
#define llround(x)      __tg_sel_r_1(llround, x)(x)
#define log10(x)        __tg_sel_r_1(log10, x)(x)
#define log1p(x)        __tg_sel_r_1(log1p, x)(x)
#define log2(x)         __tg_sel_r_1(log2, x)(x)
#define logb(x)         __tg_sel_r_1(logb, x)(x)
#define lrint(x)        __tg_sel_r_1(lrint, x)(x)
#define lround(x)       __tg_sel_r_1(lround, x)(x)
#define nearbyint(x)    __tg_sel_r_1(nearbyint, x)(x)
#define nextafter(x, y) __tg_sel_r_2(nextafter, x, y)((x), (y))
#define nexttoward(x,y) __tg_sel_r_1(nexttoward, x)((x), (y))
#define remainder(x,y)  __tg_sel_r_2(remainder, x, y)((x), (y))
#define remquo(x,y,z)   __tg_sel_r_2(remquo, x, y)((x), (y), (z))
#define rint(x)         __tg_sel_r_1(rint, x)(x)
#define round(x)        __tg_sel_r_1(round, x)(x)
#define scalbn(x,y)     __tg_sel_r_1(scalbn, x)((x), (y))
#define scalbln(x,y)    __tg_sel_r_1(scalbln, x)((x), (y))
#define tgamma(x)       __tg_sel_r_1(tgamma, x)(x)
#define trunc(x)        __tg_sel_r_1(trunc, x)(x)

#define asinpi(x)               __tg_sel_r_1(asinpi, x)(x)
#define acospi(x)               __tg_sel_r_1(acospi, x)(x)
#define atanpi(x)               __tg_sel_r_1(atanpi, x)(x)
#define atan2pi(x,y)            __tg_sel_r_2(atan2pi, x, y)((x), (y))
#define sinpi(x)                __tg_sel_r_1(sinpi, x)(x)
#define cospi(x)                __tg_sel_r_1(cospi, x)(x)
#define tanpi(x)                __tg_sel_r_1(tanpi, x)(x)
#define exp10(x)                __tg_sel_r_1(exp10, x)(x)
#define exp10m1(x)              __tg_sel_r_1(exp10m1, x)(x)
#define log10p1(x)              __tg_sel_r_1(log10p1, x)(x)
#define log2p1(x)               __tg_sel_r_1(log2p1, x)(x)
#define compoundn(x, y)         __tg_sel_r_1(compoundn, x)((x), (y))
#define pown(x, y)              __tg_sel_r_1(pown, x)((x), (y))
#define powr(x, y)              __tg_sel_r_2(powr, x, y)((x), (y))
#define rootn(x, y)             __tg_sel_r_1(rootn, x)((x), (y))
#define rsqrt(x)                __tg_sel_r_1(rsqrt, x)(x)
#define roundeven(x)            __tg_sel_r_1(roundeven, x)(x)
#define fromfp(x, y, z)         __tg_sel_r_1(fromfp, x)((x), (y), (z))
#define ufromfp(x, y, z)        __tg_sel_r_1(ufromfp, x)((x), (y), (z))
#define fromfpx(x, y, z)        __tg_sel_r_1(fromfpx, x)((x), (y), (z))
#define ufromfpx(x, y, z)       __tg_sel_r_1(ufromfpx, x)((x), (y), (z))
#define nextup(x)               __tg_sel_r_1(nextup, x)(x)
#define nextdown(x)             __tg_sel_r_1(nextdown, x)(x)
#define fmaximum(x, y)          __tg_sel_r_2(fmaximum, x, y)((x), (y))
#define fminimum(x, y)          __tg_sel_r_2(fminimum, x, y)((x), (y))
#define llogb(x)                __tg_sel_r_1(llogb, x)(x)
#define fmaximum_mag(x,y)       __tg_sel_r_2(fmaximum_mag, x, y)((x), (y))
#define fmaximum_num(x,y)       __tg_sel_r_2(fmaximum_num, x, y)((x), (y))
#define fmaximum_mag_num(x,y)   __tg_sel_r_2(fmaximum_mag_num, x, y)((x), (y))
#define fminimum_mag(x,y)       __tg_sel_r_2(fminimum_mag, x, y)((x), (y))
#define fminimum_num(x,y)       __tg_sel_r_2(fminimum_num, x, y)((x), (y))
#define fminimum_mag_num(x,y)   __tg_sel_r_2(fminimum_mag_num, x, y)((x), (y))

#define fadd(x, y)      _Generic((x)+(y), long double: faddl, default: fadd)((x), (y))
#define fsub(x, y)      _Generic((x)+(y), long double: fsubl, default: fsub)((x), (y))    
#define fmul(x, y)      _Generic((x)+(y), long double: fmull, default: fmul)((x), (y))
#define fdiv(x, y)      _Generic((x)+(y), long double: fdivl, default: fdiv)((x), (y))
#define ffma(x, y, z)   _Generic((x)+(y)+(z), long double: ffmal, default: ffma)((x), (y))
#define fsqrt(x)        _Generic((x), long double: fsqrtl, default: fsqrt)(x)
#define dadd(x, y)      daddl(x, y)
#define dsub(x, y)      dsubl(x, y)
#define dmul(x, y)      dmull(x, y)
#define ddiv(x, y)      ddivl(x, y)
#define dfma(x, y)      dfmal(x, y)
#define dsqrt(x)        dsqrtl(x)



#define __tg_sel_c_1(stem, x) _Generic((x), complex long double: stem##l, complex double: stem, complex float: stem##f, long double: stem##l, float: stem##f, default: stem)

#define carg(x) __tg_sel_c_1(carg, x)(x)
#define creal(x) __tg_sel_c_1(creal, x)(x)
#define cimag(x) __tg_sel_c_1(cimag, x)(x)
#define conj(x) __tg_sel_c_1(conj, x)(x)
#define cproj(x) __tg_sel_c_1(cproj, x)(x)
#endif
