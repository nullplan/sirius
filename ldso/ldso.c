#include "cpu.h"

_Noreturn void _start_c(long *sp, long *dynv, long base)
{
    for (;;)
        a_crash();
}
