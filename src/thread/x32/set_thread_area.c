#include "pthread.h"
#include <syscall.h>

int __set_thread_area(uintptr_t x)
{
    return __syscall(SYS_arch_prctl, 0x1002, x);
}
