#include <math.h>
#pragma STDC FENV_ACCESS ON

long double scalbnl(long double x, int n)
{
    return scalblnl(x, n);
}
weak_alias(ldexpl, scalbnl);
