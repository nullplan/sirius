#include <spawn.h>
#include <errno.h>

int posix_spawnattr_setflags(posix_spawnattr_t *psa, short flags)
{
    if (flags >= 32u)
        return EINVAL;
    psa->__flg = flags;
    return 0;
}
