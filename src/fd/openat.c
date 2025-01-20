#include <fcntl.h>
#include <stdarg.h>
int openat(int dfd, const char *name, int flags, ...)
{
    mode_t m = 0;
    if (flags & (O_CREAT|__O_TMPFILE)) {
        va_list ap;
        va_start(ap, flags);
        m = va_arg(ap, mode_t);
        va_end(ap);
    }
    return __openat(dfd, name, flags, m);
}
