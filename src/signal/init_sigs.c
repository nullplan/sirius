#include <signal.h>
#include "syscall.h"

static const unsigned long __int_sigs[] = {
#ifdef __SIXTY_FOUR
    0x0000000380000000,
#if _NSIG > 65
    0,
#endif
#else
    0x80000000, 3,
#if _NSIG > 65
    0, 0,
#endif
#endif
};

hidden void __init_sigs(void)
{
    __syscall(SYS_rt_sigprocmask, SIG_UNBLOCK, __int_sigs, 0, _NSIG/8);
}

