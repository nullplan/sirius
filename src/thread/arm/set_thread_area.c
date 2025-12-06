#include <pthread.h>
#include "syscall.h"
#include "cpu.h"
#include <sys/auxv.h>

extern hidden size_t __a_barrier_ptr;
extern hidden size_t __a_cas_ptr;
extern hidden size_t __a_gettp_ptr;
extern hidden char __a_barrier_old[];
extern hidden char __a_barrier_v6[];
extern hidden char __a_barrier_v7[];
extern hidden char __a_cas_v6[];
extern hidden char __a_cas_v7[];
extern hidden char __a_gettp_v6[];

int __set_thread_area(uintptr_t x)
{
    #if __ARM_ARCH < 7
    size_t hwcap = __getauxval(AT_HWCAP);
    if (hwcap & 0x8000) {
        const char *platform = (const char *)__getauxval(AT_PLATFORM);
        /* kernel platforms are all "v<MAJOR><add-on><endian>", where <MAJOR> is a decimal number.
         * At the moment, they are all single-digit, and it's unlikely to rise higher.
         */
        /* If TLS is set, use v6 for everything unless v7 or higher is affirmatively detected */
        __a_gettp_ptr = (size_t)__a_gettp_v6;
        if (platform && platform[0] == 'v' && platform[1] > '6') {
            __a_barrier_ptr = (size_t)__a_barrier_v7;
            __a_cas_ptr = (size_t)__a_cas_v7;
        } else {
            __a_barrier_ptr = (size_t)__a_barrier_v6;
            __a_cas_ptr = (size_t)__a_cas_v6;
        }
    } else {
        int ver = *(int *)0xffff0ffc;
        if (ver < 2) a_crash();
        __a_gettp_ptr = 0xffff0fe0;
        __a_cas_ptr = 0xffff0fc0;
        if (ver < 3) __a_barrier_ptr = (size_t)__a_barrier_old;
        else __a_barrier_ptr = 0xffff0fa0;
    }
    #endif
    return __syscall(0xf0005, x);
}
