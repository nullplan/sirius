#include <fenv.h>
#include "fenv-x86.h"

int fegetenv(fenv_t *fe)
{
    fe->__sw = __sw();
    fe->__cw = __cw();
    fe->__mxcsr = __mxcsr();
    return 0;
}
