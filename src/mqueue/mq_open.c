#include <mqueue.h>
#include <stdarg.h>
#include "syscall.h"

mqd_t mq_open(const char *name, int flags, ...)
{
    va_list ap;
    mode_t m = 0;
    struct mq_attr *attr = 0;
    if (flags & O_CREAT)
    {
        va_start(ap, flags);
        m = va_arg(ap, mode_t);
        attr = va_arg(ap, void *);
        va_end(ap);
    }
    return syscall(SYS_mq_open, name, flags, m, attr);
}
