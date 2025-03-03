#include <strings.h>
#include "cpu.h"

int ffsll(long long x)
{
    if (!x) return 0;
    return a_ctz_64(x);
}
