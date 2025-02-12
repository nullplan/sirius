#include <math.h>

float scalbnf(float x, int n)
{
    return scalblnf(x, n);
}
weak_alias(ldexpf, scalbnf);
