#ifndef __LIMITS_H
#define __LIMITS_H

#include <alltypes.h>

#define CHAR_BIT        8

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
