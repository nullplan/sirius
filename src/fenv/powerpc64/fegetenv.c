#include <fenv.h>
#include "fenv-ppc.h"

int fegetenv(fenv_t *fe)
{
    fe->__fpscr = __fpscr();
    return 0;
}
