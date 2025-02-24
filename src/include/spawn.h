#ifndef SPAWN_H
#define SPAWN_H

#include "../../include/spawn.h"
#include <sched.h>

enum {
    FDOP_OPEN,
    FDOP_CLOSE,
    FDOP_DUP2,
    FDOP_CHDIR,
    FDOP_FCHDIR,
};

struct __fa {
    struct __fa *next;
    int fdop;
    int fd, fd2;
    int mode;
    char name[];
};

hidden int __spawn(pid_t *restrict, const char *restrict, const posix_spawn_file_actions_t *restrict, const posix_spawnattr_t *restrict, char *const [restrict], char *const [restrict], int (*)(const char *, char *const *, char *const *));
#endif
