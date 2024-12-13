#ifndef __STDARG_H
#define __STDARG_H

#define __NEED_va_list
#include <alltypes.h>

#define va_arg __builtin_va_arg
#define va_start __builtin_va_start
#define va_copy __builtin_va_copy
#define va_end __builtin_va_end

#endif
