#include <unistd.h>
#include "syscall.h"

int link(const char *a, const char *b)
{
#ifdef SYS_link
    return syscall(SYS_link, a, b);
#else
    return syscall(SYS_linkat, -100, a, -100, b, 0);
#endif
}
