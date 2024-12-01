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
double  sin(double);
double  cos(double);
double  tan(double);
double asin(double);
double acos(double);
double atan(double);
double atan2(double, double);
double  sinh(double);
double  cosh(double);
double  tanh(double);
double asinh(double);
double acosh(double);
double atanh(double);
double exp(double);
double exp2(double);
double expm1(double);
double frexp(double, int *);
int ilogb(double);
double ldexp(double, int);
double log(double);
double log10(double);
double log1p(double);
double log2(double);
double logb(double);
double modf(double, double *);
double scalbn(double, int);
double scalbln(double, long);
double cbrt(double);
double fabs(double);
double hypot(double, double);
double pow(double, double);
double sqrt(double);
double erf(double);
double erfc(double);
double lgamma(double);
double tgamma(double);
double ceil(double);
double floor(double);
double nearbyint(double);
double rint(double);
long lrint(double);
long long llrint(double);
double round(double);
long lround(double);
long long llround(double);
double trunc(double);
double fmod(double, double);
double remainder(double, double);
double remquo(double, double, int *);
double copysign(double, double);
double nan(const char *);
double nextafter(double, double);
double nexttoward(double, long double);
double fdim(double, double);
double fmax(double, double);
double fmin(double, double);
double fma(double, double, double);

#ifdef __cplusplus
}
#endif
#endif
