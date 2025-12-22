#include <math.h>
#pragma STDC FENV_ACCESS ON

float scalbnf(float x, int n)
{
    return scalblnf(x, n);
}
weak_alias(ldexpf, scalbnf);
