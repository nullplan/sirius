#include <stdlib.h>
#include <spawn.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <pthread.h>
#include "libc.h"

int system(const char *cmd)
{
    if (!cmd) return 1;

    struct sigaction ign = {.sa_handler = SIG_IGN};
    struct sigaction oldint, oldquit;
    sigset_t ss, restore, reset;
    int cs;

    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &cs);

    sigaction(SIGINT, &ign, &oldint);
    sigaction(SIGQUIT, &ign, &oldquit);
    sigemptyset(&ss);
    sigaddset(&ss, SIGCHLD);
    pthread_sigmask(SIG_BLOCK, &ss, &restore);

    posix_spawnattr_t spawnattr;
    posix_spawnattr_init(&spawnattr);
    sigemptyset(&reset);
    if (oldint.sa_handler != SIG_IGN) sigaddset(&reset, SIGINT);
    if (oldquit.sa_handler != SIG_IGN) sigaddset(&reset, SIGQUIT);

    posix_spawnattr_setflags(&spawnattr, POSIX_SPAWN_SETSIGMASK | POSIX_SPAWN_SETSIGDEF);
    posix_spawnattr_setsigmask(&spawnattr, &restore);
    posix_spawnattr_setsigdefault(&spawnattr, &reset);
    pid_t pid;
    int rv = posix_spawn(&pid, "/bin/sh", 0, &spawnattr, (char *[]){"sh", "-c", "--", (char *)cmd, 0}, __environ);
    int stat = -1;
    posix_spawnattr_destroy(&spawnattr);

    if (!rv) while (waitpid(pid, &stat, 0) < 0 && errno == EINTR);
    pthread_sigmask(SIG_SETMASK, &restore, 0);
    sigaction(SIGQUIT, &oldquit, 0);
    sigaction(SIGINT, &oldint, 0);
    pthread_setcancelstate(cs, 0);
    return stat;
}
