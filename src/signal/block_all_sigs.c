#include <signal.h>
#include "syscall.h"

static const unsigned long long __all_sigs[] = {
    -1ull,
#if _NSIG > 65
    -1ull,
#endif
};

hidden int __block_all_sigs(sigset_t *oss)
{
    return __syscall(SYS_rt_sigprocmask, SIG_BLOCK, __all_sigs, oss, _NSIG/8);
}

