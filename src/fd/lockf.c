#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int lockf(int fd, int cmd, off_t len)
{
    int rv;
    struct flock fl = {.l_type = cmd == F_ULOCK? F_UNLCK : F_WRLCK,
        .l_whence = SEEK_CUR,
        .l_len = len,
    };

    if (cmd == F_TEST) {
        rv = fcntl(fd, F_GETLK, &fl);
        if (!rv && fl.l_type != F_UNLCK && fl.l_pid != getpid()) {
            errno = EAGAIN;
            rv = -1;
        }
        return rv;
    }

    rv = fcntl(fd, cmd == F_LOCK? F_SETLKW : F_SETLK, &fl);
    return rv;
}
