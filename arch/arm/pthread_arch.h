#include <stddef.h>
extern hidden size_t __a_gettp_ptr;

#if __ARM_ARCH < 5
#define BLX "mov lr,pc; bx "
#else
#define BLX "blx "
#endif

static inline uintptr_t __get_tp(void)
{
    register uintptr_t r0 __asm__("r0");
    __asm__(BLX "%1" : "=r"(r0) : "r"(__a_gettp_ptr));
    return r0;
}
#define REG_IP arm_pc
