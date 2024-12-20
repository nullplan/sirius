#ifndef __LIMITS_H
#define __LIMITS_H

#define __STDC_VERSION_LIMITS_H__ 202311L

#include <alltypes.h>
#include <features.h>

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

#define _POSIX_CLOCKRES_MIN     20000000
#define _POSIX_AIO_LISTIO_MAX   2
#define _POSIX_AIO_MAX          1
#define _POSIX_ARG_MAX          4096
#define _POSIX_CHILD_MAX 25
#define _POSIX_DELAYTIMER_MAX 32
#define _POSIX_HOST_NAME_MAX 255
#define _POSIX_LINK_MAX 8
#define _POSIX_LOGIN_NAME_MAX 9
#define _POSIX_MAX_CANON 255
#define _POSIX_MAX_INPUT 255
#define _POSIX_MQ_OPEN_MAX 8
#define _POSIX_MQ_PRIO_MAX 32
#define _POSIX_NAME_MAX 14
#define _POSIX_NGROUPS_MAX 8
#define _POSIX_OPEN_MAX 20
#define _POSIX_PATH_MAX 256
#define _POSIX_PIPE_BUF 512
#define _POSIX_RE_DUP_MAX 255
#define _POSIX_RTSIG_MAX 8
#define _POSIX_SEM_NSEMS_MAX 256
#define _POSIX_SEM_VALUE_MAX 32767
#define _POSIX_SIGQUEUE_MAX 32
#define _POSIX_SSIZE_MAX 32767
#define _POSIX_STREAM_MAX 8
#define _POSIX_SYMLINK_MAX 255
#define _POSIX_SYMLOOP_MAX 8
#define _POSIX_THREAD_DESTRUCTOR_ITERATIONS 4
#define _POSIX_THREAD_KEYS_MAX 128
#define _POSIX_THREAD_THREADS_MAX 64
#define _POSIX_TIMER_MAX 32
#define _POSIX_TTY_NAME_MAX 9
#define _POSIX_TZNAME_MAX 6
#define _POSIX2_BC_BASE_MAX 99
#define _POSIX2_BC_DIM_MAX 2048
#define _POSIX2_BC_SCALE_MAX 99
#define _POSIX2_BC_STRING_MAX 1000
#define _POSIX2_CHARCLASS_NAME_MAX 14
#define _POSIX2_COLL_WEIGHTS_MAX 2
#define _POSIX2_EXPR_NEST_MAX 32
#define _POSIX2_LINE_MAX 2048
#define _POSIX2_RE_DUP_MAX 255
#define _XOPEN_IOV_MAX 16
#define _XOPEN_NAME_MAX 255
#define _XOPEN_PATH_MAX 1024

#ifdef _POSIX_C_SOURCE
#include <bits/limits.h>
#define DELAYTIMER_MAX  0x7fffffff
#define HOST_NAME_MAX   64
#define LOGIN_NAME_MAX  255
#define MQ_PRIO_MAX     32768
#define OPEN_MAX        0x80000000U
#define PTHREAD_DESTRUCTOR_ITERATIONS   8
#define PTHREAD_KEYS_MAX    256
#define PTHREAD_STACK_MIN   64
#define RTSIG_MAX           30
#define SEM_VALUE_MAX       0x7fffffff
#define STREAM_MAX          1024
#define SYMLOOP_MAX         40
#define TTY_NAME_MAX        21  /* "/dev/pts/" + an int */
#define TZNAME_MAX          16
#define FILESIZEBITS        64
#define LINK_MAX            127
#define MAX_CANON           255
#define MAX_INPUT           255
#define NAME_MAX            255
#define PATH_MAX            4096
#define PIPE_BUF            4096
#define SYMLINK_MAX         4096
#define TEXTDOMAIN_MAX      252

#define BC_BASE_MAX         99
#define BC_DIM_MAX          2048
#define BC_SCALE_MAX        99
#define BC_STRING_MAX       1000
#define CHARCLASS_NAME_MAX  14
#define COLL_WEIGHTS_MAX    2
#define EXPR_NEST_MAX       2048
#define RE_DUP_MAX          255
#define LINE_MAX            2048
#define NGROUPS_MAX         65536
#define GETENTROPY_MAX      256
#define NL_ARGMAX           9
#define NL_MSGMAX           32767
#define NL_SETMAX           255
#define NSIG_MAX            65
#endif
#ifdef _XOPEN_SOURCE
#define IOV_MAX         1024
#ifdef PAGESIZE
#define PAGE_SIZE PAGESIZE
#endif
#define NL_LANGMAX          14
#define NZERO               20
#endif
#endif
