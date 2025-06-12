#include <stdlib.h>

void *malloc(size_t x)
{
    return __libc_malloc(x);
}
