#ifndef LIBM_H
#define LIBM_H

#include <math.h>
#include <float.h>
#include <stdint.h>

#if LDBL_MANT_DIG==64
union ldshape {
    long double f;
    struct {
        uint64_t mant;
        uint16_t se;
    } i;
};
#define mantzero(x) (!((x).i.mant & 0x7fffffffffffffffULL))
#elif LDBL_MANT_DIG==113
union ldshape {
    long double f;
    struct {
        uint64_t mantlo;
        uint32_t mantmid;
        uint16_t manthi;
        uint16_t se;
    } i;
};
#define mantzero(x) (!((x).i.mantlo | (x).i.mantmid | (x).i.manthi))
#endif

#define __double_from_bits(x) ((union {uint64_t __i; double __d;}){(x)}.__d)
#define __float_from_bits(x)  ((union {uint32_t __i; float __f;}){(x)}.__f)

#ifdef __GNUC__
#define unlikely(x) __builtin_expect((x), 0)
#define likely(x) __builtin_expect((x), 1)
#else
#define unlikely(x) (x)
#define likely(x) (x)
#endif

static inline void force_evalf(float v)
{
    volatile float x;
    x = v;
}

static inline void force_evald(double v)
{
    volatile double x;
    x = v;
}

static inline void force_evall(long double v)
{
    volatile long double x;
    x = v;
}

#define FORCE_EVAL(x) (sizeof (x) == sizeof (float)? force_evalf(x) : \
        sizeof (x) == sizeof (double)? force_evald(x) : force_evall(x))

struct rempio2 {
    int n;
    double val[2];
};

extern hidden struct rempio2 __rem_pio2(double);
extern hidden double __kernel_sin(double, double, int);
extern hidden double __kernel_cos(double, double);
extern hidden double __kernel_tan(double, double, int);
extern hidden int __kernel_rem_pio2(double *, double *, int, int, int, const int *);

extern hidden const uint8_t __sqrt_data[96];

extern hidden double __pzero(double);
extern hidden double __qzero(double);
extern hidden double __pone(double);
extern hidden double __qone(double);
#endif
