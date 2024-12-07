#ifndef __MATH_H
#define __MATH_H

#define __NEED_float_t
#define __NEED_double_t
#define __NEED_size_t
#define __NEED_uint32_t
#define __NEED_uint64_t
#include <alltypes.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
static inline uint64_t __double_bits(double __d)
{
    extern void *memcpy(const void *, void *, size_t);
    uint64_t __rv;
    memcpy(&__rv, &__d, sizeof __rv);
    return __rv;
}

static inline uint32_t __float_bits(float __d)
{
    extern void *memcpy(const void *, void *, size_t);
    uint32_t __rv;
    memcpy(&__rv, &__d, sizeof __rv);
    return __rv;
}
#else
#define __double_bits(x) ((union {double __d; uint64_t __i;}){(x)}.__i)
#define __float_bits(x)  ((union {float __f; uint32_t __i;}){(x)}.__i)
#endif

#ifdef __GNUC__
#define HUGE_VAL    (__builtin_inf())
#define HUGE_VALL   (__builtin_infl())
#define HUGE_VALF   (__builtin_inff())
#define NAN         (__builtin_nanf(""))
#else
#define HUGE_VAL    1e10000
#define HUGE_VALF   1e10000f
#define HUGE_VALL   1e10000l
#define NAN         (0.0f/0.0f)
#endif
#define INFINITY    HUGE_VALF

#define FP_ZERO         0
#define FP_SUBNORMAL    1
#define FP_NORMAL       2
#define FP_INFINITE     3
#define FP_NAN          4

#define FP_ILOGB0       (-0x7fffffff)
#define FP_ILOGBNAN     0x7fffffff

#define MATH_ERRNO      1
#define MATH_ERREXCEPT  2
#define math_errhandling    2

#define __fpclassifyf(x)    ((x) == 0? FP_ZERO : (x) < (1 << 23)? FP_SUBNORMAL : (x) < (0xff << 23)? FP_NORMAL : (x) == (0xff << 23)? FP_INFINITE : FP_NAN)
#define __fpclassifyd(x)    ((x) == 0? FP_ZERO : (x) < (1ull << 52)? FP_SUBNORMAL : (x) < (0x7ff << 52)? FP_NORMAL : (x) == (0x7ff << 52)? FP_INFINITE : FP_NAN)
int __fpclassifyl(long double);
int __signbitl(long double);

#define fpclassify(x) (sizeof (x) == sizeof (float)? __fpclassifyf(__float_bits(x) & 0x7fffffff) : sizeof (x) == sizeof (double)? __fpclassifyd(__double_bits(x) & 0x7fffffffffffffffULL) : __fpclassifyl(x))
#define isfinite(x) (sizeof (x) == sizeof (float)? (__float_bits(x) << 1) < (0xff << 24) : sizeof (x) == sizeof (double)? (__double_bits(x) << 1) < (0x7ffULL << 53) : __fpclassifyl(x) < FP_INFINITE)
#define isinf(x) (sizeof (x) == sizeof (float)? (__float_bits(x) << 1) == (0xff << 24) : sizeof (x) == sizeof (double)? (__double_bits(x) << 1) == (0x7ffULL << 53) : __fpclassifyl(x) == FP_INFINITE)
#define isnan(x) (sizeof (x) == sizeof (float)? (__float_bits(x) << 1) > (0xff << 24) : sizeof (x) == sizeof (double)? (__double_bits(x) << 1) > (0x7ffULL << 53) : __fpclassifyl(x) > FP_INFINITE)
#define isnormal(x) (sizeof (x) == sizeof (float)? (__float_bits(x) << 1) - (1u << 24) < (0xffu << 24) - (1u << 24) : \
        sizeof (x) == sizeof (double)? (__double_bits(x) << 1) - (1ull << 53) < (0x7ffULL << 53) - (1ull << 53) : __fpclassifyl(x) == FP_NORMAL)
#define signbit(x) (sizeof (x) == sizeof (float)? (int)(__float_bits(x) >> 31) : sizeof (x) == sizeof (double)? (int)(__double_bits(x) >> 63) : __signbitl(x))

#define isunordered(x, y) (isnan(x) || isnan(y))
#define __iscmp(x, y, c) (!isunordered(x) && (x) c (y))
#define isgreater(x, y)         __iscmp(x, y, >)
#define isgreaterequal(x, y)    __iscmp(x, y, >=)
#define isless(x, y)            __iscmp(x, y, <)
#define islessequal(x, y)       __iscmp(x, y, <=)
#define islessgreater(x, y)     __iscmp(x, y, !=)
int         ilogb(double);
long        lrint(double);
long long  llrint(double);
long       lround(double);
long long llround(double);
double        sin(double);
double        cos(double);
double        tan(double);
double       asin(double);
double       acos(double);
double       atan(double);
double       sinh(double);
double       cosh(double);
double       tanh(double);
double      asinh(double);
double      acosh(double);
double      atanh(double);
double        exp(double);
double       exp2(double);
double      expm1(double);
double      frexp(double, int *);
double      ldexp(double, int);
double        log(double);
double      log10(double);
double      log1p(double);
double       log2(double);
double       logb(double);
double     scalbn(double, int);
double    scalbln(double, long);
double       cbrt(double);
double       fabs(double);
double       sqrt(double);
double        erf(double);
double       erfc(double);
double     lgamma(double);
double     tgamma(double);
double       ceil(double);
double      floor(double);
double  nearbyint(double);
double       rint(double);
double      round(double);
double      trunc(double);
double nexttoward(double, long double);
double      atan2(double, double);
double       modf(double, double *);
double      hypot(double, double);
double        pow(double, double);
double       fmod(double, double);
double  remainder(double, double);
double     remquo(double, double, int *);
double   copysign(double, double);
double  nextafter(double, double);
double       fdim(double, double);
double       fmax(double, double);
double       fmin(double, double);
double        fma(double, double, double);
double        nan(const char *);

int         ilogbf(float);
long        lrintf(float);
long long  llrintf(float);
long       lroundf(float);
long long llroundf(float);
float        sinf(float);
float        cosf(float);
float        tanf(float);
float       asinf(float);
float       acosf(float);
float       atanf(float);
float       sinhf(float);
float       coshf(float);
float       tanhf(float);
float      asinhf(float);
float      acoshf(float);
float      atanhf(float);
float        expf(float);
float       exp2f(float);
float      expm1f(float);
float      frexpf(float, int *);
float      ldexpf(float, int);
float        logf(float);
float      log10f(float);
float      log1pf(float);
float       log2f(float);
float       logbf(float);
float     scalbnf(float, int);
float    scalblnf(float, long);
float       cbrtf(float);
float       fabsf(float);
float       sqrtf(float);
float        erff(float);
float       erfcf(float);
float     lgammaf(float);
float     tgammaf(float);
float       ceilf(float);
float      floorf(float);
float  nearbyintf(float);
float       rintf(float);
float      roundf(float);
float      truncf(float);
float nexttowardf(float, long double);
float      atan2f(float, float);
float       modff(float, float *);
float      hypotf(float, float);
float        powf(float, float);
float       fmodf(float, float);
float  remainderf(float, float);
float     remquof(float, float, int *);
float   copysignf(float, float);
float  nextafterf(float, float);
float       fdimf(float, float);
float       fmaxf(float, float);
float       fminf(float, float);
float        fmaf(float, float, float);
float        nanf(const char *);

int         ilogbl(long double);
long        lrintl(long double);
long long  llrintl(long double);
long       lroundl(long double);
long long llroundl(long double);
long double        sinl(long double);
long double        cosl(long double);
long double        tanl(long double);
long double       asinl(long double);
long double       acosl(long double);
long double       atanl(long double);
long double       sinhl(long double);
long double       coshl(long double);
long double       tanhl(long double);
long double      asinhl(long double);
long double      acoshl(long double);
long double      atanhl(long double);
long double        expl(long double);
long double       exp2l(long double);
long double      expm1l(long double);
long double      frexpl(long double, int *);
long double      ldexpl(long double, int);
long double        logl(long double);
long double      log10l(long double);
long double      log1pl(long double);
long double       log2l(long double);
long double       logbl(long double);
long double     scalbnl(long double, int);
long double    scalblnl(long double, long);
long double       cbrtl(long double);
long double       fabsl(long double);
long double       sqrtl(long double);
long double        erfl(long double);
long double       erfcl(long double);
long double     lgammal(long double);
long double     tgammal(long double);
long double       ceill(long double);
long double      floorl(long double);
long double  nearbyintl(long double);
long double       rintl(long double);
long double      roundl(long double);
long double      truncl(long double);
long double nexttowardl(long double, long double);
long double      atan2l(long double, long double);
long double       modfl(long double, long double *);
long double      hypotl(long double, long double);
long double        powl(long double, long double);
long double       fmodl(long double, long double);
long double  remainderl(long double, long double);
long double     remquol(long double, long double, int *);
long double   copysignl(long double, long double);
long double  nextafterl(long double, long double);
long double       fdiml(long double, long double);
long double       fmaxl(long double, long double);
long double       fminl(long double, long double);
long double        fmal(long double, long double, long double);
long double        nanl(const char *);

#ifdef __cplusplus
}
#endif
#endif
