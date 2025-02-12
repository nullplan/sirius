#include <math.h>
double scalbn(double x, int n)
{
    return scalbln(x, n);
}
weak_alias(ldexp, scalbn);
