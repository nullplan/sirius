#define __STARTUP_CODE
#include <pthread.h>
#include "syscall.h"

int __set_thread_area(uintptr_t x)
{
    unsigned desc[4];
    desc[0] = -1ul;
    desc[1] = x;
    desc[2] = 0xfffff;
    desc[3] = 0x51;
    int rv = __syscall(SYS_set_thread_area, desc);
    if (!rv) {
        __asm__("movw %w0, %%gs" :: "r"(8 * desc[0] + 3) : "memory");
    }
    return rv;
}
