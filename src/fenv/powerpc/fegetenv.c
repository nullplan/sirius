#include <fenv.h>
#include "fenv-ppc.h"

int fegetenv(fenv_t *fe)
{
    #ifdef __NO_FPRS__
    fe->__fpscr = 0;
    #else
    fe->__fpscr = __fpscr();
    #endif
    return 0;
}
