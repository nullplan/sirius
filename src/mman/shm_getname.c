#include <sys/mman.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

hidden int __shm_getname(char *dst, const char *src)
{
    while (*src == '/') src++;
    size_t l = strnlen(src, NAME_MAX);
    if (l == 0 || memchr(src, '/', l)) {
        errno = EINVAL;
        return -1;
    }
    if (l >= NAME_MAX) {
        errno = ENAMETOOLONG;
        return -1;
    }
    memcpy(dst, "/dev/shm/", 9);
    memcpy(dst + 9, src, l + 1);
    return 0;
}
