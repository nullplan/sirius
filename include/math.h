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

#ifdef _XOPEN_SOURCE
#define M_E             0x1.5bf0a8b145769p1
#define M_EGAMMA        0x1.2788cfc6fb619p-1
#define M_LOG2E         0x1.71547652b82fep0
#define M_LOG10E        0x1.bcb7b1526e50ep-2
#define M_LN2           0x1.62e42fefa39efp-1
#define M_LN10          0x1.26bb1bbb55516p1
#define M_PHI           0x1.9e3779b97f4a8p0
#define M_PI            0x1.921fb54442d18p1
#define M_PI_2          0x1.921fb54442d18p0
#define M_PI_4          0x1.921fb54442d18p-1
#define M_1_PI          0x1.45f306dc9c883p-2
#define M_1_SQRTPI      0x1.20dd750429b6dp-1
#define M_2_PI          0x1.45f306dc9c883p-1
#define M_2_SQRTPI      0x1.20dd750429b6dp0
#define M_SQRT2         0x1.6a09e667f3bcdp0
#define M_SQRT3         0x1.bb67ae8584caa7p0
#define M_SQRT1_2       0x1.6a09e667f3bcdp-1
#define M_SQRT1_3       0x1.279a74590331cp-1
#define M_El            0x1.5bf0a8b1457695355fb8ac404e7ap1L
#define M_EGAMMAl       0x1.2788cfc6fb618f49a37c7f0202a6p-1L
#define M_LOG2El        0x1.71547652b82fe1777d0ffda0d23ap0L
#define M_LOG10El       0x1.bcb7b1526e50e32a6ab7555f5a68p-2L
#define M_LN2l          0x1.62e42fefa39ef35793c7673007e6p-1L
#define M_LN10l         0x1.26bb1bbb5551582dd4adac5705a6p1L
#define M_PHIl          0x1.9e3779b97f4a7c15f39cc0605ceep0L
#define M_PIl           0x1.921fb54442d18469898cc51701b8p1L
#define M_PI_2l         0x1.921fb54442d18469898cc51701b8p0L
#define M_PI_4l         0x1.921fb54442d18469898cc51701b8p-1L
#define M_1_PIl         0x1.45f306dc9c882a53f84eafa3ea6ap-2L
#define M_1_SQRTPIl     0x1.20dd750429b6d11ae3a914fed7fep-1L
#define M_2_PIl         0x1.45f306dc9c882a53f84eafa3ea6ap-1L
#define M_2_SQRTPIl     0x1.20dd750429b6d11ae3a914fed7fep0L
#define M_SQRT2l        0x1.6a09e667f3bcc908b2fb1366ea95p0L
#define M_SQRT3l        0x1.bb67ae8584caa73b25742d7078b8p0L
#define M_SQRT1_2l      0x1.6a09e667f3bcc908b2fb1366ea95p-1L
#define M_SQRT1_3l      0x1.279a74590331c4d218f81e4afb25p-1L

double j0(double);
double j1(double);
double jn(int, double);
double y0(double);
double y1(double);
double yn(int, double);
extern int signgam;
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

#define FP_INT_TONEAREST    0
#define FP_INT_TONEARESTFROMZERO    1
#define FP_INT_TOWARDZERO   2
#define FP_INT_UPWARD       3
#define FP_INT_DOWNWARD     4

#define FP_ILOGB0       (-0x7fffffff)
#define FP_ILOGBNAN     0x7fffffff
#ifndef __SIXTY_FOUR
#define FP_LLOGB0       FP_ILOGB0
#define FP_LLOGBNAN     FP_ILOGNAN
#else
#define FP_LLOGB0       (-0x7fffffffffffffffL)
#define FP_LLOGBNAN     0x7fffffffffffffffL
#endif

#define MATH_ERRNO      1
#define MATH_ERREXCEPT  2
#define math_errhandling    2

#define __fpclassifyf(x)    ((x) == 0? FP_ZERO : (x) < (1 << 23)? FP_SUBNORMAL : (x) < (0xff << 23)? FP_NORMAL : (x) == (0xff << 23)? FP_INFINITE : FP_NAN)
#define __fpclassifyd(x)    ((x) == 0? FP_ZERO : (x) < (1ull << 52)? FP_SUBNORMAL : (x) < (0x7ffull << 52)? FP_NORMAL : (x) == (0x7ffull << 52)? FP_INFINITE : FP_NAN)
int __fpclassifyl(long double);
int __signbitl(long double);
int __iscanonicall(long double);

#define __iscanonical(x) ((x) <= (0x7ffULL << 52) || (x) == (0x7ff8ULL << 48))
#define __iscanonicalf(x) ((x) <= (0xff << 23) || (x) == (0xff8 << 19))

#define fpclassify(x) (sizeof (x) == sizeof (float)? __fpclassifyf(__float_bits(x) & 0x7fffffff) : sizeof (x) == sizeof (double)? __fpclassifyd(__double_bits(x) & 0x7fffffffffffffffULL) : __fpclassifyl(x))
#define isfinite(x) (sizeof (x) == sizeof (float)? (__float_bits(x) << 1) < (0xff << 24) : sizeof (x) == sizeof (double)? (__double_bits(x) << 1) < (0x7ffULL << 53) : __fpclassifyl(x) < FP_INFINITE)
#define isinf(x) (sizeof (x) == sizeof (float)? (__float_bits(x) << 1) == (0xff << 24) : sizeof (x) == sizeof (double)? (__double_bits(x) << 1) == (0x7ffULL << 53) : __fpclassifyl(x) == FP_INFINITE)
#define isnan(x) (sizeof (x) == sizeof (float)? (__float_bits(x) << 1) > (0xff << 24) : sizeof (x) == sizeof (double)? (__double_bits(x) << 1) > (0x7ffULL << 53) : __fpclassifyl(x) > FP_INFINITE)
#define isnormal(x) (sizeof (x) == sizeof (float)? (__float_bits(x) << 1) - (1u << 24) < (0xffu << 24) - (1u << 24) : \
        sizeof (x) == sizeof (double)? (__double_bits(x) << 1) - (1ull << 53) < (0x7ffULL << 53) - (1ull << 53) : __fpclassifyl(x) == FP_NORMAL)
#define signbit(x) (sizeof (x) == sizeof (float)? (int)(__float_bits(x) >> 31) : sizeof (x) == sizeof (double)? (int)(__double_bits(x) >> 63) : __signbitl(x))
#define iscanonical(x) (sizeof (x) == sizeof (float)? __iscanonicalf(__float_bits(x) << 1 >> 1) : sizeof (x) == sizeof (double)? __iscanonical(__double_bits(x) << 1 >> 1) : __iscanonicall(x))
#define issignaling(x) (0)
#define issubnormal(x) (sizeof (x) == sizeof (float)? (__float_bits(x) & 0x7fffffff) - 1u < (1 << 23) - 1u : \
        sizeof (x) == sizeof (double)? (__double_bits(x) << 1 >> 1) - 1ull < (1ull << 52) - 1ull : __fpclassifyl(x) == FP_SUBNORMAL)
#define iszero(x) (sizeof (x) == sizeof (float)? (__float_bits(x) << 1 >> 1) == 0 : sizeof (x) == sizeof (double)? (__double_bits(x) << 1 >> 1) == 0 : __fpclassifyl(x) == FP_ZERO)

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

double     asinpi(double);
double     acospi(double);
double     atanpi(double);
double    atan2pi(double, double);
double      sinpi(double);
double      cospi(double);
double      tanpi(double);
double      exp10(double);
double    exp10m1(double);
double    log10p1(double);
double     log2p1(double);
double  compoundn(double, long long);
double       pown(double, long long);
double       powr(double, double);
double      rootn(double, long long);
double      rsqrt(double);
double  roundeven(double);
double     fromfp(double, int, unsigned);
double    ufromfp(double, int, unsigned);
double    fromfpx(double, int, unsigned);
double   ufromfpx(double, int, unsigned);
double     nextup(double);
double   nextdown(double);
double   fmaximum(double, double);
double   fminimum(double, double);
long        llogb(double);
int  canonicalize(double *, const double *);
double fmaximum_mag(double, double);
double fmaximum_num(double, double);
double fmaximum_mag_num(double, double);
double fminimum_mag(double, double);
double fminimum_num(double, double);
double fminimum_mag_num(double, double);

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

float     asinpif(float);
float     acospif(float);
float     atanpif(float);
float    atan2pif(float, float);
float      sinpif(float);
float      cospif(float);
float      tanpif(float);
float      exp10f(float);
float    exp10m1f(float);
float    log10p1f(float);
float     log2p1f(float);
float  compoundnf(float, long long);
float       pownf(float, long long);
float       powrf(float, double);
float      rootnf(float, long long);
float      rsqrtf(float);
float  roundevenf(float);
float     fromfpf(float, int, unsigned);
float    ufromfpf(float, int, unsigned);
float    fromfpxf(float, int, unsigned);
float   ufromfpxf(float, int, unsigned);
float     nextupf(float);
float   nextdownf(float);
float   fmaximumf(float, float);
float   fminimumf(float, float);
float fmaximum_magf(float, float);
float fmaximum_numf(float, float);
float fmaximum_mag_numf(float, float);
float fminimum_magf(float, float);
float fminimum_numf(float, float);
float fminimum_mag_numf(float, float);
long        llogbf(float);
int  canonicalizef(float *, const float *);

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

long double     asinpil(long double);
long double     acospil(long double);
long double     atanpil(long double);
long double    atan2pil(long double, long double);
long double      sinpil(long double);
long double      cospil(long double);
long double      tanpil(long double);
long double      exp10l(long double);
long double    exp10m1l(long double);
long double    log10p1l(long double);
long double     log2p1l(long double);
long double  compoundnl(long double, long long);
long double       pownl(long double, long long);
long double       powrl(long double, double);
long double      rootnl(long double, long long);
long double      rsqrtl(long double);
long double  roundevenl(long double);
long double     fromfpl(long double, int, unsigned);
long double    ufromfpl(long double, int, unsigned);
long double    fromfpxl(long double, int, unsigned);
long double   ufromfpxl(long double, int, unsigned);
long double     nextupl(long double);
long double   nextdownl(long double);
long double   fmaximuml(long double, long double);
long double   fminimuml(long double, long double);
long double fmaximum_magl(long double, long double);
long double fmaximum_numl(long double, long double);
long double fmaximum_mag_numl(long double, long double);
long double fminimum_magl(long double, long double);
long double fminimum_numl(long double, long double);
long double fminimum_mag_numl(long double, long double);
long        llogbl(long double);
int  canonicalizel(long double *, const long double *);

float fadd(double, double);
float faddl(long double, long double);
double daddl(long double, long double);
float fsub(double, double);
float fsubl(long double, long double);
double dsubl(long double, long double);
float fmul(double, double);
float fmull(long double, long double);
double dmull(long double, long double);
float fdiv(double, double);
float fdivl(long double, long double);
double ddivl(long double, long double);
float ffma(double, double, double);
float ffmal(long double, long double, long double);
double dfmal(long double, long double, long double);
float fsqrt(double);
float fsqrtl(long double);
double dsqrtl(long double);

#ifdef __STDC_WANT_IEC_60559_EXT__
int    totalorder(const double *, const double *);
int totalordermag(const double *, const double *);
double getpayload(const double *);
int    setpayload(      double *, double);
int setpayloadsig(      double *, double);

int    totalorderf(const float *, const float *);
int totalordermagf(const float *, const float *);
float  getpayloadf(const float *);
int    setpayloadf(      float *, float);
int setpayloadsigf(      float *, float);

int    totalorderl(const long double *, const long double *);
int totalordermagl(const long double *, const long double *);
long double getpayloadl(const long double *);
int    setpayloadl(      long double *, long double);
int setpayloadsigl(      long double *, long double);
#endif

#ifdef __cplusplus
}
#endif
#endif
