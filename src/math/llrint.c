#include "libm.h"
#pragma STDC FENV_ACCESS ON
long long llrint(double x)
{
    return rint(x);
}
