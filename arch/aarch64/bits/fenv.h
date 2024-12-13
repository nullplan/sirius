typedef struct { unsigned __data; } fenv_t;
typedef struct { unsigned __data; } fexcept_t;

#define FE_DIVBYZERO    1
#define FE_INEXACT      2
#define FE_INVALID      4
#define FE_OVERFLOW     8
#define FE_UNDERFLOW    16
#define FE_ALL_EXCEPT   31

#define FE_DOWNWARD     0
#define FE_TONEAREST    1
#define FE_TOWARDZERO   2
#define FE_UPWARD       3
