#include <spawn.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>

int posix_spawn_file_actions_addfchdir(posix_spawn_file_actions_t *psfa, int fd)
{
    struct __fa *p = malloc(sizeof *p);
    if (!p) return ENOMEM;
    p->next = 0;
    *psfa->__l = p;
    psfa->__l = &p->next;

    p->fdop = FDOP_FCHDIR;
    p->fd = fd;
    return 0;
}
