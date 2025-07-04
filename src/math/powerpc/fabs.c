#include <math.h>
#ifdef __NO_FPRS__
#include "../fabs.c"
#else
double fabs(double x)
{
    __asm__("fabs %0, %1" : "=d"(x) : "d"(x));
    return x;
}
#endif
