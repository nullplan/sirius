#include "libc.h"
#include <string.h>
#include <assert.h>

hidden char *__procfdname(int fd, char *b)
{
    assert(fd >= 0);
    *--b = 0;
    do *--b = fd % 10 + '0';
    while (fd /= 10);
    return memcpy(b - 14, "/proc/self/fd/", 14);

}
