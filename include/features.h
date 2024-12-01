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
#endif
