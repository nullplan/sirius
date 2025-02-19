#include <stdlib.h>

static void dummy(void) {}
weak_alias(__run_quick_exit_funcs, dummy);

_Noreturn void quick_exit(int st)
{
    __run_quick_exit_funcs();
    _Exit(st);
}
