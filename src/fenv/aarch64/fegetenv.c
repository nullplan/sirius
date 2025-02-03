#include <fenv.h>
#include "fenv-ppc.h"

int fegetenv(fenv_t *fe)
{
    fe->__fpsr = __fpsr();
    fe->__fpcr = __fpcr();
    return 0;
}
