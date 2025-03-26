#include <unistd.h>
#include <sys/ioctl.h>
#include "syscall.h"
#include "libc.h"
#include <stdio.h>
#include <sys/stat.h>

int ttyname_r(int fd, char *name, size_t namesize)
{
    int len;
    int n;
    int rv = __syscall(SYS_ioctl, TIOCGPTN, &n);
    if (!rv)
        len = snprintf(name, namesize, "/dev/pts/%d", n);       /* XXX: Improve this! */
    else {
        if (!isatty(fd)) return errno;

        char pfdbuf[PROCFD_LEN];
        char *p = __procfdname(fd, pfdbuf + sizeof pfdbuf);
        len = readlink(p, name, namesize);
        if (len < 0) return errno;
        if (len == namesize) return ERANGE;
        name[len] = 0;
    }

    struct stat linkstat, fdstat;
    if (stat(name, &linkstat) || fstat(fd, &fdstat))
        return errno;
    if (linkstat.st_dev != fdstat.st_dev || linkstat.st_ino != fdstat.st_ino)
        return ENODEV;
    if (len >= namesize)
        return ERANGE;
    return 0;
}
