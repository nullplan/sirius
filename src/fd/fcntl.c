#include <fcntl.h>
#include <stdarg.h>
#include "syscall.h"

int fcntl(int fd, int cmd, ...)
{
    va_list ap;
    int rv;
    struct f_owner_ex oex;
    long arg = 0;

    va_start(ap, cmd);
    switch (cmd) {
        case F_DUPFD:
        case F_DUPFD_CLOEXEC:
        case F_SETFD:
        case F_SETFL:
        case F_SETOWN:
        case F_SETSIG:
            arg = va_arg(ap, int);
            break;

        case F_GETFD:
        case F_GETFL:
        case F_GETOWN:
        case F_GETSIG:
            break;

        default:
            arg = (long)va_arg(ap, void *);
            break;
    }
    va_end(ap);

    /* Operational challenges with the fcntl() syscall:
     * F_DUPFD_CLOEXEC: Not supported by all kernel versions.
     * And even where supported, in some versions they forgot to
     * make the new file descriptor CLOEXEC.
     *
     * F_GETOWN: If the owner is a process group, this can return a negative
     * value. Unfortunately, in all syscall ABIs, negative values are error codes.
     * Workaround: Use F_GETOWN_EX if supported.
     * Else use F_GETFD to verify the file descriptor, then use F_GETOWN's raw return code.
     */
    if (cmd == F_DUPFD_CLOEXEC) {
        rv = __syscall(SYS_fcntl, fd, F_DUPFD_CLOEXEC, arg);
        if (rv < 0) rv = __syscall(SYS_fcntl, fd, F_DUPFD, arg);
        if (rv >= 0) __syscall(SYS_fcntl, rv, F_SETFD, FD_CLOEXEC);
    } else if (cmd == F_GETOWN) {
        rv = __syscall(SYS_fcntl, fd, F_GETOWN_EX, &oex);
        if (rv == 0) return oex.type == F_OWNER_PGRP? -oex.pid : oex.pid;
        rv = __syscall(SYS_fcntl, fd, F_GETFD);
        if (rv >= 0)
            return __syscall(SYS_fcntl, fd, F_GETOWN);
    } else if (cmd == F_SETLKW || cmd == F_OFD_SETLKW) {
        rv = __syscall_cp(SYS_fcntl, fd, cmd, arg);
    } else {
        rv = __syscall(SYS_fcntl, fd, cmd, arg);
    }
    return __syscall_ret(rv);
}
