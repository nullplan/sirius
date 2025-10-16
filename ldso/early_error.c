/* error message printer for code running before TD is setup. */
#define __STARTUP_CODE
#include <string.h>
#include "syscall.h"

/* error printing is strictly best-effort */
hidden _Noreturn void __die_early_multi(const char **msgs, size_t n)
{
    for (size_t i = 0; i < n; i++)
        __syscall(SYS_write, 2, msgs[i], strlen(msgs[i]));
    for (;;)
        __syscall(SYS_exit, 1);
}

hidden _Noreturn void __die_early(const char *msg)
{
    __die_early_multi(&msg, 1);
}
