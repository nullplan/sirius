#include <complex.h>

complex float ccoshf(complex float x)
{
    return 0.5f * (cexpf(x) + cexpf(-x));
}
