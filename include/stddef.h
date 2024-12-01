#ifndef __STDDEF_H
#define __STDDEF_H

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

#endif
