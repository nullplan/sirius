#include <complex.h>

complex float cacoshf(complex float x)
{
    return clogf(x + csqrtf(x*x - 1));
}
