typedef struct { unsigned __fpscr; } fenv_t;
typedef struct { unsigned __fpscr; } fexcept_t;

#define FE_INVALID      0x20000000
#define FE_OVERFLOW     0x10000000
#define FE_UNDERFLOW    0x08000000
#define FE_DIVBYZERO    0x04000000
#define FE_INEXACT      0x02000000
#define FE_ALL_EXCEPT   0x3e000000

/* PPC specific extension */
#define FE_INVALID_SNAN                 0x01000000
#define FE_INVALID_ISI                  0x00800000
#define FE_INVALID_IDI                  0x00400000
#define FE_INVALID_ZDZ                  0x00200000
#define FE_INVALID_IMZ                  0x00100000
#define FE_INVALID_COMPARE              0x00080000
#define FE_INVALID_SOFTWARE             0x00000400
#define FE_INVALID_SQRT                 0x00000200
#define FE_INVALID_INTEGER_CONVERSION   0x00000100
#define FE_ALL_INVALID                  0x01f80700

#define FE_TONEAREST    0
#define FE_TOWARDZERO   1
#define FE_UPWARD       2
#define FE_DOWNWARD     3
