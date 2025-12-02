static inline unsigned __fpscr(void)
{
    unsigned rv;
    __asm__("vmrs %0, FPSCR" : "=r"(rv) :: "memory");
    return rv;
}

static inline void __set_fpscr(unsigned val)
{
    __asm__("vmsr FPSCR, %0" :: "r"(val) : "memory");
}
