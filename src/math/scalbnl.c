#include <math.h>

long double scalbnl(long double x, int n)
{
    return scalblnl(x, n);
}
weak_alias(ldexpl, scalbnl);
