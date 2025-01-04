#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

_Noreturn void __assert_fail(const char *cond, const char *func, const char *file, int line)
{
    fprintf(stderr, "%s:%d: In function ` %s': Assertion `%s' failed\n", file, line, func, cond);
    fflush(stderr);
    abort();
}

