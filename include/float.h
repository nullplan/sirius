#ifndef __FLOAT_H
#define __FLOAT_H

#include <alltypes.h>

#ifdef __cplusplus
extern "C"
#endif
int __flt_rounds(void);

#define FLT_ROUNDS  (__flt_rounds())
#define FLT_RADIX   2

#define DECIMAL_DIG     LDBL_DECIMAL_DIG

#define FLT_MANT_DIG    24
#define FLT_DIG         6
#define FLT_MIN_EXP     (-125)
#define FLT_MIN_10_EXP  (-37)
#define FLT_MAX_EXP     128
#define FLT_MAX_10_EXP  38
#define FLT_MAX         0x1.fffffep127f
#define FLT_EPSILON     0x1.0p-23f
#define FLT_MIN         0x1.0p-126f

#define DBL_MANT_DIG    53
#define DBL_DIG         15
#define DBL_MIN_EXP     (-1021)
#define DBL_MIN_10_EXP  (-307)
#define DBL_MAX_EXP     1024
#define DBL_MAX_10_EXP  308
#define DBL_MAX         0x1.fffffffffffffp1023
#define DBL_EPSILON     0x1.0p-52
#define DBL_MIN         0x1.0p-1022

#include <bits/float.h>
#endif
