#include <spawn.h>
#include <unistd.h>

int posix_spawn(pid_t *restrict pid, const char *restrict name,
        const posix_spawn_file_actions_t *restrict psfa,
        const posix_spawnattr_t *restrict psa,
        char *const args[restrict], char *const env[restrict])
{
    return __spawn(pid, name, psfa, psa, args, env, execve);
}
