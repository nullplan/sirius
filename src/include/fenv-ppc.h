#include <stdint.h>
static inline uint32_t __fpscr(void) {
    union {
        double d;
        uint64_t i;
    } u;
    __asm__("mffs %0" : "=f"(u.d) :: "memory");
    return u.i;
}

static inline void __set_fpscr(uint32_t val) {
    union {
        double d;
        uint64_t i;
    } u = {.i = val};
    __asm__("mtfsf 255, %0" :: "f"(u.d) : "memory");
}
