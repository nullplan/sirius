#include <strings.h>
#include "cpu.h"

int ffsl(long x)
{
    if (!x) return 0;
    return a_ctz(x);
}
