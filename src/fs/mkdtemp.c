#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

char *mkdtemp(char *template)
{
    size_t templen = strlen(template);
    if (templen < 6 || memcmp(template + templen - 6, "XXXXXX", 6))
    {
        errno = EINVAL;
        return 0;
    }

    int rv;
    for (int attempt = 0; attempt < 100; attempt++)
    {
        __randname(template + templen - 6);
        rv = mkdir(template, 0700);
        if (rv == 0 || errno != EEXIST)
            break;
    }
    return rv? 0 : template;
}
