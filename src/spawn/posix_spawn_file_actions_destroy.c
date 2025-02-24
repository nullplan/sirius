#include <spawn.h>
#include <stdlib.h>

int posix_spawn_file_actions_destroy(posix_spawn_file_actions_t *psfa)
{
    for (struct __fa *node = psfa->__f, *next; node; node = next)
    {
        next = node->next;
        free(node);
    }
    return 0;
}
