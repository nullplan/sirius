#include <stdlib.h>
#include "libc.h"

static void dummy(void) {}
weak_alias(__run_atexit_funcs, dummy);
weak_alias(__stdio_exit, dummy);
_Noreturn void exit(int code)
{
    __run_atexit_funcs();
    __run_destructors();
    __stdio_exit();
    _Exit(code);
}
