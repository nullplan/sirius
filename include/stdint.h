#ifndef __STDINT_H
#define __STDINT_H

#define __STDC_VERSION_STDINT_H__ 202311L

#define __NEED_WCHAR_MIN
#define __NEED_uint8_t
#define __NEED_uint16_t
#define __NEED_uint32_t
#define __NEED_uint64_t
#define __NEED_int8_t
#define __NEED_int16_t
#define __NEED_int32_t
#define __NEED_int64_t
#define __NEED_uint_fast16_t
#define __NEED_uint_fast32_t
#define __NEED_int_fast16_t
#define __NEED_int_fast32_t
#include <alltypes.h>

typedef int8_t int_least8_t;
typedef int16_t int_least16_t;
typedef int32_t int_least32_t;
typedef int64_t int_least64_t;
typedef uint8_t  uint_least8_t;
typedef uint16_t uint_least16_t;
typedef uint32_t uint_least32_t;
typedef uint64_t uint_least64_t;
typedef int8_t int_fast8_t;
typedef uint8_t uint_fast8_t;
typedef int64_t int_fast64_t;
typedef uint64_t uint_fast64_t;

typedef long intptr_t;
typedef unsigned long uintptr_t;

typedef int64_t intmax_t;
typedef uint64_t uintmax_t;

#define INT8_WIDTH  8
#define INT16_WIDTH 16
#define INT32_WIDTH 32
#define INT64_WIDTH 64
#define UINT8_WIDTH  8
#define UINT16_WIDTH 16
#define UINT32_WIDTH 32
#define UINT64_WIDTH 64
#define  INT_LEAST8_WIDTH  8
#define  INT_LEAST16_WIDTH 16
#define  INT_LEAST32_WIDTH 32
#define  INT_LEAST64_WIDTH 64
#define UINT_LEAST8_WIDTH  8
#define UINT_LEAST16_WIDTH 16
#define UINT_LEAST32_WIDTH 32
#define UINT_LEAST64_WIDTH 64
#define  INT_FAST8_WIDTH  8
#define  INT_FAST16_WIDTH __WIDTH_INT_FAST16
#define  INT_FAST32_WIDTH __WIDTH_INT_FAST32
#define  INT_FAST64_WIDTH 64
#define UINT_FAST8_WIDTH  8
#define UINT_FAST16_WIDTH __WIDTH_INT_FAST16
#define UINT_FAST32_WIDTH __WIDTH_INT_FAST32
#define UINT_FAST64_WIDTH 64

#define INT8_MAX    0x7f
#define INT16_MAX   0x7fff
#define INT32_MAX   0x7fffffff
#define INT64_MAX   0x7fffffffffffffffLL

#define INT8_MIN    (-INT8_MAX-1)
#define INT16_MIN   (-INT16_MAX-1)
#define INT32_MIN   (-INT32_MAX-1)
#define INT64_MIN   (-INT64_MAX-1)

#define UINT8_MAX   (2*INT8_MAX+1)
#define UINT16_MAX  (2*INT16_MAX+1)
#define UINT32_MAX  (2U*INT32_MAX+1)
#define UINT64_MAX  (2ULL*INT64_MAX+1)

#define INT_LEAST8_MAX  INT8_MAX
#define INT_LEAST16_MAX INT16_MAX
#define INT_LEAST32_MAX INT32_MAX
#define INT_LEAST64_MAX INT64_MAX
#define  INT_LEAST8_MIN  INT8_MIN
#define INT_LEAST16_MIN INT16_MIN
#define INT_LEAST32_MIN INT32_MIN
#define INT_LEAST64_MIN INT64_MIN
#define UINT_LEAST8_MAX  UINT8_MAX
#define UINT_LEAST16_MAX UINT16_MAX
#define UINT_LEAST32_MAX UINT32_MAX
#define UINT_LEAST64_MAX UINT64_MAX

#define INT_FAST8_MAX   INT8_MAX
#define INT_FAST8_MIN   INT8_MIN
#define UINT_FAST8_MAX  UINT8_MAX

#define INT_FAST64_MAX  INT64_MAX
#define INT_FAST64_MIN  INT64_MIN
#define UINT_FAST64_MAX  UINT64_MAX

#if __WIDTH_INT_FAST16 == 16
#define INT_FAST16_MAX  INT16_MAX
#define INT_FAST16_MIN  INT16_MIN
#define UINT_FAST16_MAX UINT16_MAX
#elif __WIDTH_INT_FAST16 == 32
#define INT_FAST16_MAX   INT32_MAX
#define INT_FAST16_MIN   INT32_MIN
#define UINT_FAST16_MAX UINT32_MAX
#else
#define INT_FAST16_MAX   INT64_MAX
#define INT_FAST16_MIN   INT64_MIN
#define UINT_FAST16_MAX UINT64_MAX
#endif

#if __WIDTH_INT_FAST32 == 32
#define  INT_FAST32_MAX  INT32_MAX
#define  INT_FAST32_MIN  INT32_MIN
#define UINT_FAST32_MAX UINT32_MAX
#else
#define  INT_FAST32_MAX  INT64_MAX
#define  INT_FAST32_MIN  INT64_MIN
#define UINT_FAST32_MAX UINT64_MAX
#endif

#ifndef __SIXTY_FOUR
#define INTPTR_WIDTH 32
#define UINTPTR_WIDTH 32
#define INTPTR_MIN  INT32_MIN
#define INTPTR_MAX  INT32_MAX
#define UINTPTR_MAX UINT32_MAX
#else
#define INTPTR_WIDTH 64
#define UINTPTR_WIDTH 64
#define INTPTR_MIN  INT64_MIN
#define INTPTR_MAX  INT64_MAX
#define UINTPTR_MAX UINT64_MAX
#endif

#define INTMAX_WIDTH 64
#define UINTMAX_WIDTH 64
#define INTMAX_MAX  INT64_MAX
#define INTMAX_MIN  INT64_MIN
#define UINTMAX_MAX UINT64_MAX

#define PTRDIFF_WIDTH INTPTR_WIDTH
#define PTRDIFF_MIN INTPTR_MIN
#define PTRDIFF_MAX INTPTR_MAX
#define SIG_ATOMIC_WIDTH 32
#define SIG_ATOMIC_MIN  INT32_MIN
#define SIG_ATOMIC_MAX  INT32_MAX
#define SIZE_WIDTH INTPTR_WIDTH
#define SIZE_MAX    UINTPTR_MAX
#define WCHAR_WIDTH 32
#define WINT_WIDTH 32
#define WINT_MIN INT32_MIN
#define WINT_MAX INT32_MAX

#define INT8_C(x)   x
#define INT16_C(x)  x
#define INT32_C(x)  x
#define INT64_C(x)  x##LL
#define UINT8_C(x)  x
#define UINT16_C(x) x
#define UINT32_C(x) x##U
#define UINT64_C(x) x##ULL
#define INTMAX_C(x) x##LL
#define UINTMAX_C(x) x##ULL
#endif
