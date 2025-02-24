#include <spawn.h>
#include <signal.h>
#include <string.h>

int posix_spawnattr_setsigmask(posix_spawnattr_t *restrict psa, const sigset_t *restrict mask)
{
    memcpy(psa->__msk, mask, _NSIG/8);
    return 0;
}
