#include <spawn.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>

int posix_spawn_file_actions_addopen(posix_spawn_file_actions_t *restrict psfa, int fd, const char *restrict name, int flags, mode_t mode)
{
    size_t namelen = strnlen(name, PATH_MAX);
    if (namelen >= PATH_MAX) return ENAMETOOLONG;
    struct __fa *p = malloc(sizeof *p + namelen + 1);
    if (!p) return ENOMEM;
    p->next = 0;
    *psfa->__l = p;
    psfa->__l = &p->next;

    p->fdop = FDOP_OPEN;
    p->fd = fd;
    p->fd2 = flags;
    p->mode = mode;
    memcpy(p->name, name, namelen + 1);
    return 0;
}
