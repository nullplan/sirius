#include <stdlib.h>
#include <unistd.h>
#include "syscall.h"

_Noreturn void _Exit(int code)
{
    __syscall(SYS_exit_group, code);
    for (;;)
        __syscall(SYS_exit, code);
}
weak_alias(_exit, _Exit);
