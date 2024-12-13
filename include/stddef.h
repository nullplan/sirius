#ifndef __STDDEF_H
#define __STDDEF_H

#define __STDC_VERSION_STDDEF_H__ 202311L

#define __NEED_ptrdiff_t
#define __NEED_size_t
#define __NEED_wchar_t
#define __NEED_NULL
#include <alltypes.h>

#ifdef __GNUC__
#define offsetof(t, m) __builtin_offsetof(t, m)
#else
#define offsetof(t, m) ((char *)&(((t *)0)->m) - (char *)0)
#endif

#if __STDC_VERSION__ > 201100L
typedef struct { _Alignas(16) char __x[16]; } max_align_t;
#endif

#if __STDC_VERSION__ >= 202311L
#define unreachable() __builtin_unreachable()
typedef typeof_unqual(nullptr) nullptr_t;
#endif


#endif
