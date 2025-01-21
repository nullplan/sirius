#include <fenv.h>

int feholdexcept(fenv_t *fe)
{
    /* we only support the non-stop mode */
    fegetenv(fe);
    feclearexcept(FE_ALL_EXCEPT);
    return 0;
}
