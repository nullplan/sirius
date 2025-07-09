#include <stdlib.h>
#include <errno.h>
#include <limits.h>

void *reallocarray(void *p, size_t a, size_t b)
{
    if (!b) return realloc(p, 0);
    if (a > SIZE_MAX/b) {
        errno = ENOMEM;
        return 0;
    }
    return realloc(p, a * b);
}
