#include <stdlib.h>

void free_sized(void *p, size_t sz)
{
    free(p);
}

void free_aligned_sized(void *p, size_t a, size_t sz)
{
    free(p);
}
