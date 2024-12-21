static inline uintptr_t __get_tp(void)
{
    register uintptr_t tp __asm__("r13");
    __asm__("" : "=r"(tp));
    return tp;
}
#define TP_OFFSET   0x7000
#define DTV_OFFSET  0x8000
