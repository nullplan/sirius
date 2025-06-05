#include <stdio.h>
#include <fcntl.h>
#include "syscall.h"
#include <assert.h>

FILE *freopen(const char *restrict name, const char *restrict mode, FILE *restrict oldf)
{
    FILE *rv = 0;
    __FLOCK(oldf);
    fflush(oldf);
    oldf->flags &= ~(F_ERR|F_EOF);
    int newflags = __fopen_flags(mode);
    if (newflags != -1) {
        if (name) {
            FILE *new = fopen(name, mode);
            if (new) {
                int rv = __syscall(SYS_dup3, new->fd, oldf->fd, newflags & O_CLOEXEC);
                #ifdef SYS_dup2
                if (rv == -ENOSYS)
                    __syscall(SYS_dup2, new->fd, oldf->fd);
                #endif
                if (newflags & O_CLOEXEC)
                    __syscall(SYS_fcntl, oldf->fd, F_SETFD, FD_CLOEXEC);
                oldf->dir = 0;
                oldf->lbf = new->lbf;
                oldf->read = new->read;
                oldf->write = new->write;
                oldf->seek = new->seek;
                oldf->close = new->close;
                oldf->cookie = new->cookie;
                fclose(new);
                rv = oldf;
            }
        } else {
            if (((newflags & O_ACCMODE) == O_RDONLY || !(oldf->flags & F_NOWR))
                    && ((newflags & O_ACCMODE) == O_WRONLY || !(oldf->flags & F_NORD))
                    && !__syscall(SYS_fcntl, oldf->fd, F_SETFD, newflags & O_CLOEXEC? FD_CLOEXEC : 0)
                    && ((oldf->flags & F_NOWR) || !__syscall(SYS_fcntl, oldf->fd, F_SETFL, newflags & O_APPEND)))
                rv = oldf;
        }
    }
    __FUNLOCK(oldf);
    return rv;
}
