#include <spawn.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>

int posix_spawn_file_actions_adddup2(posix_spawn_file_actions_t *psfa, int fd, int fd2)
{
    struct __fa *p = malloc(sizeof *p);
    if (!p) return ENOMEM;
    p->next = 0;
    *psfa->__l = p;
    psfa->__l = &p->next;

    p->fdop = FDOP_DUP2;
    p->fd = fd;
    p->fd2 = fd2;
    return 0;
}
