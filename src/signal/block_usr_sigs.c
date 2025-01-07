#include <signal.h>
#include "syscall.h"

static const unsigned long __usr_sigs[] = {
#ifdef __SIXTY_FOUR
    0xfffffffc7fffffff,
#if _NSIG > 65
    -1ul,
#endif
#else
    0x7fffffff, 0xfffffffc,
#if _NSIG > 65
    -1ul, -1ul,
#endif
#endif
};

hidden int __block_usr_sigs(sigset_t *oss)
{
    return __syscall(SYS_rt_sigprocmask, SIG_BLOCK, __usr_sigs, oss, _NSIG/8);
}

