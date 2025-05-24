#include <semaphore.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <fcntl.h>
#include "syscall.h"
#include <string.h>
#include <limits.h>
#include <unistd.h>

static sem_t *open_existing(const char *fullname)
{
    int fd = __syscall_ret(__sys_open(fullname, O_RDWR | O_CLOEXEC));
    if (fd == -1) return 0;
    fcntl(fd, F_SETFD, FD_CLOEXEC);

    sem_t *p = mmap(0, sizeof (sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    __syscall(SYS_close, fd);
    if (p == MAP_FAILED) return 0;
    return p;
}

static sem_t *create_new(const char *fullname, sem_t *init, mode_t m)
{
    char tmpname[] = "/dev/shm/.sem_XXXXXX";
    int fd = -1;
    for (int spins = 0; fd < 0 && spins < 100; spins++)
    {
        __randname(tmpname + sizeof tmpname - 7);
        fd = __sys_open(tmpname, O_RDWR | O_CREAT | O_EXCL | O_CLOEXEC, m);
    }
    if (fd < 0) {
        errno = EAGAIN;
        return 0;
    }
    fcntl(fd, F_SETFD, FD_CLOEXEC);
    if (write(fd, init, sizeof (sem_t)) != sizeof (sem_t))
        goto out_close;

    sem_t *p = mmap(0, sizeof (sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED) goto out_close;

    if (link(tmpname, fullname)) goto out_unmap;

    __syscall(SYS_close, fd);
#ifdef SYS_unlink
    __syscall(SYS_unlink, tmpname);
#else
    __syscall(SYS_unlinkat, AT_FDCWD, tmpname);
#endif
    return p;

out_unmap:
    __syscall(SYS_munmap, p, sizeof (sem_t));
out_close:
    __syscall(SYS_close, fd);
#ifdef SYS_unlink
    __syscall(SYS_unlink, tmpname);
#else
    __syscall(SYS_unlinkat, AT_FDCWD, tmpname);
#endif
    return 0;
}

sem_t *sem_open(const char *name, int flg, ...)
{
    while (*name == '/') name++;
    size_t len = strnlen(name, NAME_MAX + 1);
    if (len > NAME_MAX) {
        errno = ENAMETOOLONG;
        return 0;
    }
    if (!len || *name == '.' || memchr(name, '/', len)) {
        errno = EINVAL;
        return 0;
    }
    char fullnamebuf[sizeof "/dev/shm/" + NAME_MAX];
    sem_t sem;
    mode_t m;
    int initialized = 0;
    memcpy(fullnamebuf, "/dev/shm/", 9);
    memcpy(fullnamebuf + 9, name, len + 1);
    for (;;) {
        if (!(flg & O_EXCL)) {
            sem_t *p = open_existing(fullnamebuf);
            if (p || errno != ENOENT) return p;
        }
        if (flg & O_CREAT) {
            if (!initialized) {
                initialized = 1;
                va_list ap;
                va_start(ap, flg);
                m = va_arg(ap, mode_t);
                unsigned val = va_arg(ap, unsigned);
                va_end(ap);
                if (val >= SEM_VALUE_MAX) {
                    errno = EINVAL;
                    return 0;
                }
                sem_init(&sem, 1, val);
            }
            sem_t *p = create_new(fullnamebuf, &sem, m);
            if (p || errno != EEXIST) return p;
        }
        if (flg != O_CREAT) return 0;
    }
}
