#include <spawn.h>
#include <string.h>
#include <signal.h>

int posix_spawnattr_setsigdefault(posix_spawnattr_t *restrict psa, const sigset_t *restrict dfl)
{
    memcpy(psa->__dfl, dfl, _NSIG/8);
    return 0;
}
