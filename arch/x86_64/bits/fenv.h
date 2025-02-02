typedef struct { unsigned short __sw, __cw; unsigned __mxcsr; } fenv_t;
typedef struct { unsigned __mxcsr; } fexcept_t;

#define FE_INVALID      1
#define FE_DIVBYZERO    4
#define FE_OVERFLOW     8
#define FE_UNDERFLOW    16
#define FE_INEXACT      32
#define FE_ALL_EXCEPT   61

#define FE_TONEAREST    0
#define FE_DOWNWARD     0x400
#define FE_UPWARD       0x800
#define FE_TOWARDZERO   0xc00
