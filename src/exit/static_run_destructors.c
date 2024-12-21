#include "libc.h"
#include <stdint.h>
#include <stddef.h>

typedef void (*finifunc_t)(void);

extern weak finifunc_t __fini_array_start[];
extern weak finifunc_t __fini_array_end[];

static void static_run_destructors(void)
{
    size_t n = ((uintptr_t)__fini_array_end - (uintptr_t)__fini_array_start)/(sizeof (finifunc_t));
    for (finifunc_t *i = __fini_array_start; i < __fini_array_start + n; i++)
        (*i)();
}
weak_alias(__run_destructors, static_run_destructors);
