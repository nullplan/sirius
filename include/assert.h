#ifndef __ASSERT_H
#define __ASSERT_H

#define __STDC_VERSION_ASSERT_H__ 202311L

#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif
_Noreturn void __assert_fail(const char *, const char *, const char *, int);
#if __STDC_VERSION__ > 201100L && __STDC_VERSION__ < 202311L
#define static_assert _Static_assert
#endif
#ifdef __cplusplus
}
#endif
#endif

#undef assert
#ifdef NDEBUG
#define assert(x) ((void)0)
#else
#define assert(x) ((x)? ((void)0) : __assert_fail(#x, __func__, __FILE__, __LINE__))
#endif
