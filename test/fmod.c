#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fenv.h>

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <num> <num>\n", argv[0]);
        return 1;
    }

    double x = strtod(argv[1], 0);
    double y = strtod(argv[2], 0);
    feclearexcept(FE_ALL_EXCEPT);
    double z = fmod(x, y);
    int exc = fetestexcept(FE_ALL_EXCEPT);
    printf("fmod(%g, %g) = %g", x, y, z);
    if (exc & FE_INVALID)
        printf(", INVALID");
    if (exc & FE_DIVBYZERO)
        printf(", DIVBYZERO");
    if (exc & FE_OVERFLOW)
        printf(", OVERFLOW");
    if (exc & FE_UNDERFLOW)
        printf(", UNDERFLOW");
    if (exc & FE_INEXACT)
        printf(", INEXACT");
    printf("\n");
    return 0;
}
