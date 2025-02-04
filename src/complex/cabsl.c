#include <complex.h>
#include <math.h>

long double cabsl(complex long double z)
{
    return hypotl(creall(z), cimagl(z));
}
