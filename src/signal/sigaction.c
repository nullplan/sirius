#include <signal.h>
#include <string.h>
#include "syscall.h"
#include "ksig_arch.h"
#include "cpu.h"
#include "libc.h"

#ifdef SA_RESTORER
extern hidden unsigned char __sigrestore[], __rt_sigrestore[];
#endif

static unsigned long handler_set[_NSIG/8/sizeof(long)];
static void set_handler_bit(int sig)
{
    sig--;
    a_or_l(handler_set + (sig / 8 / sizeof (long)), 1ul << (sig % (8 * sizeof (long))));
}

hidden void __get_handler_set(unsigned long *p)
{
    memcpy(p, handler_set, _NSIG/8);
}

hidden int __libc_sigaction(int sig, const struct sigaction *restrict sa, struct sigaction *restrict osa)
{
    struct ksigaction ksa;
    if (sa) {
        ksa.handler = sa->sa_handler;
        memcpy(&ksa.mask, &sa->sa_mask, _NSIG/8);
        ksa.flags = sa->sa_flags;
#ifdef SA_RESTORER
        ksa.flags |= SA_RESTORER;
        ksa.restorer = ksa.flags & SA_SIGINFO? __rt_sigrestore : __sigrestore;
#endif
        if (sa->sa_handler != SIG_DFL && sa->sa_handler != SIG_IGN) {
            if (!(sa->sa_flags & SA_RESTART))
                a_swap(&__eintr_valid, 1);
            set_handler_bit(sig);
        }
    }
    int rv = __syscall(SYS_rt_sigaction, sig, sa?&ksa:0, osa?&ksa:0, _NSIG/8);
    if (!rv && osa) {
        osa->sa_handler = ksa.handler;
        osa->sa_flags = ksa.flags;
        memcpy(&osa->sa_mask, &ksa.mask, _NSIG/8);
    }
    return rv;
}

hidden int __sigaction(int sig, const struct sigaction *restrict sa, struct sigaction *restrict osa)
{
    if (sig - 1u >= _NSIG - 1u || sig - 32u < 3u) {
        errno = EINVAL;
        return -1;
    }
    sigset_t ss;
    if (sig == SIGABRT) {
        __block_usr_sigs(&ss);
        __lock(&__abort_lock);
    }
    int rv = __libc_sigaction(sig, sa, osa);
    if (sig == SIGABRT) {
        __unlock(&__abort_lock);
        __restore_sigs(&ss);
    }
    return __syscall_ret(rv);
}
weak_alias(sigaction, __sigaction);
