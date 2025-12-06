#include <stddef.h>
#if ((__ARM_ARCH == 6 && (!defined __thumb__ || defined __ARM_ARCH_6T2__)) || __ARM_ARCH > 6)
static inline uintptr_t __get_tp(void)
{
    uintptr_t rv;
    __asm__("mrc p15, 0, %0, cr13, cr0, 3" : "=r"(rv));
    return rv;
}
#else

#if __ARM_ARCH >= 5
#define BLX "blx "
#elif defined __thumb__
#define BLX "bl __thumb_bx_"
#else
#define BLX "mov lr,pc; bx "
#endif

static inline uintptr_t __get_tp(void)
{
    extern hidden size_t __a_gettp_ptr;
    register uintptr_t r0 __asm__("r0");
    __asm__(BLX "%1" : "=r"(r0) : "r"(__a_gettp_ptr));
    return r0;
}
#endif
#define REG_IP arm_pc
