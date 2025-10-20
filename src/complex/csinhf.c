#include <complex.h>

complex float csinhf(complex float x)
{
    return 0.5f * (cexpf(x) - cexpf(-x));
}
