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
#define mantzero(x) (!((x).mantlo | (x).mantmid | (x).manthi))
#endif

#define __double_from_bits(x) ((union {uint64_t __i; double __d;}){(x)}.__d)
#define __float_from_bits(x)  ((union {uint32_t __i; float __f;}){(x)}.__f)
#endif
