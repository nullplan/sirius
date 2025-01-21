#include <fenv.h>

int fegetmode(femode_t *m)
{
    m->__rm = fegetround();
    return 0;
}
