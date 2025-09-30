#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

int mkstemp(char *template)
{
    size_t templen = strlen(template);
    if (templen < 6 || memcmp(template + templen - 6, "XXXXXX", 6))
    {
        errno = EINVAL;
        return -1;
    }

    int rv;
    for (int attempt = 0; attempt < 100; attempt++)
    {
        __randname(template + templen - 6);
        rv = __openat(AT_FDCWD, template, O_RDWR | O_CREAT | O_EXCL, 0600);
        if (rv >= 0 || errno != EEXIST)
            break;
    }
    return rv;
}
