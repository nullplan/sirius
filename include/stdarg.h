#ifndef __STDARG_H
#define __STDARG_H

#define __NEED_va_list
#include <alltypes.h>

#define va_arg(a, t) __builtin_va_arg(a, t)
#define va_start(a, p) __builtin_va_start(a, p);
#define va_copy(d, s) __builtin_va_copy(d, s);
#define va_end(a) __builtin_va_end(a)

#endif
