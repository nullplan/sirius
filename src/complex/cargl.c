#include <complex.h>
#include <math.h>

long double cargl(complex long double z)
{
    return atan2l(cimagl(z), creall(z));
}
