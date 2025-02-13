static inline uintptr_t __get_tp(void)
{
    uintptr_t rv;
    __asm__("mrs %0, tpidr_el0" : "=r"(rv));
    return rv;
}
#define GAP_ABOVE_TP    16
#define REG_IP pc
