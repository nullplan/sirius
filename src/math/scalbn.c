#include <math.h>
#pragma STDC FENV_ACCESS ON
double scalbn(double x, int n)
{
    return scalbln(x, n);
}
weak_alias(ldexp, scalbn);
