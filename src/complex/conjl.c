#include <complex.h>

complex long double conjl(complex long double z)
{
    return CMPLXL(creall(z), -cimagl(z));
}
