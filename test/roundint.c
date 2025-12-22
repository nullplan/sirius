#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdnoreturn.h>
#include <fenv.h>

static noreturn void usage(const char *pn)
{
    fprintf(stderr, "Usage: %s number\n", pn);
    exit(1);
}

static const struct {
    const char *name;
    int mode;
} rounding_mode[] = {
#define FE(x) {"FE_" #x, FE_##x}
    FE(TONEAREST),
#ifdef FE_TONEARESTFROMZERO
    FE(TONEARESTFROMZERO),
#endif
#ifdef FE_DOWNWARD
    FE(DOWNWARD),
#endif
#ifdef FE_UPWARD
    FE(UPWARD),
#endif
#ifdef FE_TOWARDZERO
    FE(TOWARDZERO),
#endif
};

#define PRINT_RES(fmt, expr) do { \
    feclearexcept(FE_INEXACT | FE_OVERFLOW | FE_INVALID); \
    typeof(expr) val = expr; \
    int exc = fetestexcept(FE_INEXACT | FE_OVERFLOW | FE_INVALID); \
    printf(fmt, x, val); \
    if (exc & FE_INEXACT) \
        printf(", INEXACT"); \
    if (exc & FE_OVERFLOW) \
        printf(", OVERFLOW"); \
    if (exc & FE_INVALID) \
        printf(", INVALID"); \
    printf("\n"); \
} while (0)

int main(int argc, char **argv)
{
    if (argc < 2) usage(argv[0]);
    char *endp;
    double x = strtod(argv[1], &endp);
    if (!*argv[1] || *endp) usage(argv[0]);
    for (size_t i = 0; i < sizeof rounding_mode / sizeof *rounding_mode; i++)
    {
        printf("%s:\n", rounding_mode[i].name);
        if (fesetround(rounding_mode[i].mode))
            printf("could not be set!\n");
        else {
            PRINT_RES("floor(%g)     = % g", floor(x));
            PRINT_RES("ceil(%g)      = % g", ceil(x));
            PRINT_RES("trunc(%g)     = % g", trunc(x));
            PRINT_RES("round(%g)     = % g", round(x));
            PRINT_RES("roundeven(%g) = % g", roundeven(x));
            PRINT_RES("lround(%g)    = % ld", lround(x));
            PRINT_RES("llround(%g)   = % lld", llround(x));
            PRINT_RES("rint(%g)      = % g", rint(x));
            PRINT_RES("lrint(%g)     = % ld", lrint(x));
            PRINT_RES("llrint(%g)    = % lld", llrint(x));
        }
    }
    return 0;
}
