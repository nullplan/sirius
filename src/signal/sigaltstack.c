#include <signal.h>
#include "syscall.h"

int sigaltstack(const stack_t *restrict st, stack_t *restrict ost)
{
    return syscall(SYS_sigaltstack, st, ost);
}
