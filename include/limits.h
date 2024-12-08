#ifndef __LIMITS_H
#define __LIMITS_H

#define __STDC_VERSION_LIMITS_H__ 202311L

#include <alltypes.h>

#define BOOL_WIDTH      8
#define CHAR_BIT        8
#define USHRT_WIDTH     16
#define UINT_WIDTH      16
#ifndef __SIXTY_FOUR
#define ULONG_WIDTH     32
#else
#define ULONG_WIDTH     64
#endif
#define ULLONG_WIDTH    64
#define BITINT_MAXWIDTH ULLONG_WIDTH

#define BOOL_MAX        0xff
#define UCHAR_WIDTH     CHAR_BIT
#define SCHAR_WIDTH     CHAR_BIT
#define SHRT_WIDTH      USHRT_WIDTH
#define INT_WIDTH       UINT_WIDTH
#define LONG_WIDTH      ULONG_WIDTH
#define LLONG_WIDTH     ULLONG_WIDTH

#define SCHAR_MAX       0x7f
#define SHRT_MAX        0x7fff
#define INT_MAX         0x7fffffff
#ifndef __SIXTY_FOUR
#define LONG_MAX        0x7fffffffL
#else
#define LONG_MAX        0x7fffffffffffffffL
#endif
#define LLONG_MAX       0x7fffffffffffffffL

#define SCHAR_MIN       (-SCHAR_MAX-1)
#define SHRT_MIN        (-SHRT_MAX-1)
#define INT_MIN         (-INT_MAX-1)
#define LONG_MIN        (-LONG_MAX-1)
#define LLONG_MIN       (-LLONG_MAX-1)

#define UCHAR_MAX       (2*SCHAR_MAX+1)
#define USHRT_MAX       (2*SHRT_MAX+1)
#define UINT_MAX        (2U*INT_MAX+1)
#define ULONG_MAX       (2UL*LONG_MAX+1)
#define ULLONG_MAX      (2ULL*LLONG_MAX+1)

#define MB_LEN_MAX      4

#if '\xff' < 0
#define CHAR_MIN    SCHAR_MIN
#define CHAR_MAX    SCHAR_MAX
#else
#define CHAR_MIN    0
#define CHAR_MAX    UCHAR_MAX
#endif

#endif
