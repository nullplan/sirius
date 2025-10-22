#include <complex.h>

complex float catanhf(complex float x)
{
    return 0.5 * (clogf(1 + x) - clogf(1 - x));
}
