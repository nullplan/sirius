#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdnoreturn.h>
#include <fenv.h>

static noreturn void usage(const char *pn)
{
    fprintf(stderr, "Usage: %s number rnd width\n", pn);
    exit(1);
}


int main(int argc, char **argv)
{
    if (argc < 4) usage(argv[0]);
    char *endp;
    double x = strtod(argv[1], &endp);
    if (!*argv[1] || *endp) usage(argv[0]);
    int rnd = strtol(argv[2], &endp, 10);
    if (!*argv[2] || *endp) usage(argv[0]);
    unsigned width = strtoul(argv[3], &endp, 10);
    if (!*argv[3] || *endp) usage(argv[0]);
    feclearexcept(FE_INEXACT | FE_OVERFLOW | FE_INVALID);
    double val = fromfp(x, rnd, width);
    int exc = fetestexcept(FE_INEXACT | FE_OVERFLOW | FE_INVALID);
    printf("fromfp(%g, %d, %u) = %g", x, rnd, width, val);
    if (exc & FE_INEXACT)
        printf(", INEXACT");
    if (exc & FE_OVERFLOW)
        printf(", OVERFLOW");
    if (exc & FE_INVALID)
        printf(", INVALID");
    printf("\n");
    return 0;
}
