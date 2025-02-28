#include <strings.h>
#include "cpu.h"

int ffs(int x)
{
    if (!x) return 0;
    return a_ctz(x);
}
