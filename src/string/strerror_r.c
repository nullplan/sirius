#include <string.h>
#include <errno.h>

int strerror_r(int e, char *buf, size_t n)
{
    const char *msg = strerror(e);
    size_t len = strlen(msg);
    if (len >= n) return ERANGE;
    memcpy(buf, msg, len + 1);
    return 0;
}
