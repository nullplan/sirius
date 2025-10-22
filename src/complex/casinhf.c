#include <complex.h>

complex float casinhf(complex float x)
{
    return clogf(x + csqrtf(x*x + 1));
}
