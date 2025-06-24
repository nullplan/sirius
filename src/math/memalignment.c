#include <stdlib.h>
size_t memalignment(const void *p)
{
    size_t x = (size_t)p;
    return x & -x;
}
