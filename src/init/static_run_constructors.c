#include "libc.h"
#include <stddef.h>
#include <stdint.h>

typedef void (*initfunc_t)(void);

extern weak initfunc_t __init_array_start[];
extern weak initfunc_t __init_array_end[];
extern weak initfunc_t __preinit_array_start[];
extern weak initfunc_t __preinit_array_end[];

static void run_funcs(initfunc_t *start, initfunc_t *end)
{
    size_t n = ((uintptr_t)end - (uintptr_t)start)/(sizeof (initfunc_t));
    for (initfunc_t *i = start; i < start + n; i++)
        (*i)();
}

static void static_run_constructors(void)
{
    run_funcs(__preinit_array_start, __preinit_array_end);
    run_funcs(__init_array_start, __init_array_end);
}
weak_alias(__run_constructors, static_run_constructors);
