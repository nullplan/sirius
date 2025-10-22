#include <complex.h>

complex double catanh(complex double x)
{
    return 0.5 * (clog(1 + x) - clog(1 - x));
}
