#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <syscall.h>

#define ATTEMPTS    10000
FILE *tmpfile(void)
{
    int fd;
    fd = __sys_open("/tmp", O_RDWR | O_TMPFILE, 0666);
    if (fd < 0) {
        char name[] = "/tmp/tmpfile_XXXXXX";
        for (size_t i = 0; i < ATTEMPTS; i++) {
            __randname(name + sizeof name - 7);
            fd = __sys_open(name, O_RDWR | O_CREAT | O_EXCL, 0666);
            if (fd == -EEXIST) continue;
            if (fd < 0) return 0;
#ifdef SYS_unlink
            __syscall(SYS_unlink, name);
#else
            __syscall(SYS_unlinkat, AT_FDCWD, name);
#endif
            break;
        }
    }
    if (fd < 0) return 0;
    FILE *f = __fdopen_flg(fd, O_RDWR);
    if (f) return f;
    __syscall(SYS_close, fd);
    return 0;
}
