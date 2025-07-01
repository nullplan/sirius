#ifndef __FEATURES_H
#define __FEATURES_H

#ifdef __cplusplus
#define __restrict
#else
#define __restrict restrict
#endif

#if __STDC_VERSION__ > 202300L || __cplusplus > 201100L
#define _Noreturn [[noreturn]]
#elif defined __cplusplus || (defined __STDC_VERSION__ && __STDC_VERSION__ < 201100L)
#  ifdef __GNUC__
#  define _Noreturn __attribute__((noreturn))
#  else
#  define _Noreturn
#  endif
#endif

#if __STDC_VERSION > 202300L
#define __unsequenced [[unsequenced]]
#else
#define __unsequenced
#endif

#ifdef __GNUC__
#define _Alignas(x) __attribute__((aligned(x)))
#elif __STDC_VERSION__ > 201100L
/* just keep _Alignas around */
#elif __cplusplus > 201100L
#define _Alignas(x) alignas(x)
#else
#define _Alignas(x)
#endif

#if __STDC_VERSION__ >= 202311L
#define __deprecated [[deprecated]]
#else
#define __deprecated
#endif

#ifdef __GNUC__
#define __ret_twice __attribute__((returns_twice))
#else
#define __ret_twice
#endif

#if defined _GNU_SOURCE && !defined _BSD_SOURCE
#define _BSD_SOURCE
#endif

#if defined _BSD_SOURCE && !defined _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif

#if defined _XOPEN_SOURCE && !defined _POSIX_C_SOURCE
#define _POSIX_C_SOURCE
#endif
#endif
