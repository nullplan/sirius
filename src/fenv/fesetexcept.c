#include <fenv.h>

int fesetexcept(int x)
{
    /* we only support non-stop environments, and therefore: */
    return feraiseexcept(x);
}
