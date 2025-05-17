static inline uintptr_t __get_tp(void)
{
    uintptr_t rv;
    __asm__("movl %%fs:0, %0" : "=r"(rv));
    return rv;
}
#define TLS_VARIANT_2
#define CANARY_PAD
#define REG_IP rip
