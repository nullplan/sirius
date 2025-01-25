#include <fenv.h>

int feupdateenv(const fenv_t *fe)
{
    int x = fetestexcept(FE_ALL_EXCEPT);
    fesetenv(fe);
    feraiseexcept(x);
    return 0;
}
