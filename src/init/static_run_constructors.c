#include "libc.h"
#include <stddef.h>
#include <stdint.h>

typedef void (*initfunc_t)(void);

extern weak initfunc_t __init_array_start[];
extern weak initfunc_t __init_array_end[];

static void static_run_constructors(void)
{
    size_t n = ((uintptr_t)__init_array_end - (uintptr_t)__init_array_start)/(sizeof (initfunc_t));
    for (initfunc_t *i = __init_array_start; i < __init_array_start + n; i++)
        (*i)();
}
weak_alias(__run_constructors, static_run_constructors);
