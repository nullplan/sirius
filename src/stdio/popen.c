#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <spawn.h>
#include <errno.h>

FILE *popen(const char *cmd, const char *mode)
{
    if ((*mode != 'r' && *mode != 'w')
            || (mode[1] != 0 && (mode[1] != 'e' || mode[2])))
    {
        errno = EINVAL;
        return 0;
    }

    int p[2];
    if (pipe2(p, O_CLOEXEC))
        return 0;
    if (p[0] < 1) {
        int fd = fcntl(p[0], F_DUPFD_CLOEXEC, 2);
        close(p[0]);
        if (fd < 0) {
            close(p[1]);
            return 0;
        }
        p[0] = fd;
    }
    if (p[1] < 1) {
        int fd = fcntl(p[1], F_DUPFD_CLOEXEC, 2);
        close(p[1]);
        if (fd < 0) {
            close(p[0]);
            return 0;
        }
        p[1] = fd;
    }

    int dir = *modestr == 'w';

    FILE *ret = __fdopen_flg(p[dir], mode);
    if (!ret) {
        close(p[0]);
        close(p[1]);
        return 0;
    }

    posix_spawn_file_actions_t psfa;
    if (!posix_spawn_file_actions_init(&psfa)) {
        if (posix_spawn_file_actions_adddup2(&psfa, p[!dir], !dir))
            goto fail;

        FILE *head = __ofl_lock();
        for (FILE *f = head; f; f = f->next) {
            if (!f->popen_pid) continue;
            if (posix_spawn_file_actions_addclose(&psfa, f->fd)) {
fail_unlock:
                __ofl_unlock(head);
fail:
                posix_spawn_file_actions_destroy(&psfa);
                fclose(ret);
                close(p[!dir]);
                return 0;
            }
        }
        pid_t pid;
        int stat = posix_spawn(&pid, "/bin/sh", &psfa, 0, (char *[]){"sh", "-c", "--", (char *)cmd, 0}, __environ);
        if (!stat)
            f->popen_pid = pid;
        else
        {
            errno = stat;
            goto fail_unlock;
        }

        __ofl_unlock(head);
        posix_spawn_file_actions_destroy(&psfa);
    }
    close(p[!dir]);
    if (mode[1] != 'e')
        fcntl(p[dir], F_SETFD, 0);
    return ret;

}
