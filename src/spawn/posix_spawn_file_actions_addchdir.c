#include <spawn.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>

int posix_spawn_file_actions_addchdir(posix_spawn_file_actions_t *restrict psfa, const char *restrict name)
{
    size_t namelen = strnlen(name, PATH_MAX);
    if (namelen >= PATH_MAX) return ENAMETOOLONG;
    struct __fa *p = malloc(sizeof *p + namelen + 1);
    if (!p) return ENOMEM;
    p->next = 0;
    *psfa->__l = p;
    psfa->__l = &p->next;

    p->fdop = FDOP_CHDIR;
    memcpy(p->name, name, namelen + 1);
    return 0;
}
