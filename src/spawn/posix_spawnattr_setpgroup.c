#include <spawn.h>

int posix_spawnattr_setpgroup(posix_spawnattr_t *psa, pid_t pgrp)
{
    psa->__pgrp = pgrp;
    return 0;
}
