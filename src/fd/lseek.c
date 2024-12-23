#include <unistd.h>
#include <syscall.h>

hidden off_t __lseek(int fd, off_t off, int s)
{
#ifdef SYS__llseek
    off_t rv;
    if (syscall(SYS__llseek, fd, off >> 32, off, &rv, s))
        rv = -1;
#else
    off_t rv = syscall(SYS_lseek, fd, off, s);
#endif
    return rv;
}
weak_alias(lseek, __lseek);
