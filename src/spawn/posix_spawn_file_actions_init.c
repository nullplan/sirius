#include <spawn.h>

int posix_spawn_file_actions_init(posix_spawn_file_actions_t *psfa)
{
    psfa->__f = 0;
    psfa->__l = &psfa->__f;
    return 0;
}
