#include <spawn.h>
#include <string.h>
#include <signal.h>

int posix_spawnattr_getflags(const posix_spawnattr_t *restrict psa, short *restrict flg)
{
    *flg = psa->__flg;
    return 0;
}

int posix_spawnattr_getpgroup(const posix_spawnattr_t *restrict psa, pid_t *restrict pg)
{
    *pg = psa->__pgrp;
    return 0;
}

int posix_spawnattr_getsigdefault(const posix_spawnattr_t *restrict psa, sigset_t *restrict def)
{
    memcpy(def, psa->__dfl, _NSIG/8);
    return 0;
}

int posix_spawnattr_getsigmask(const posix_spawnattr_t *restrict psa, sigset_t *restrict mask)
{
    memcpy(mask, psa->__msk, _NSIG/8);
    return 0;
}
