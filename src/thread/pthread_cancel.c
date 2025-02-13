#define _GNU_SOURCE
#include <pthread.h>
#include <stddef.h>
#include <signal.h>
#include <string.h>
#include "syscall.h"
#include "libc.h"
#include "cpu.h"

extern hidden long __syscall_cp_asm(syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, int *);
extern hidden const char __cp_begin[];
extern hidden const char __cp_end[];
extern hidden const char __cp_cancel[];

hidden _Noreturn void __cancel(void)
{
    pthread_exit(PTHREAD_CANCELED);
}

hidden long __syscall_cp_c(syscall_arg_t a, syscall_arg_t b, syscall_arg_t c, syscall_arg_t d, syscall_arg_t e, syscall_arg_t f, syscall_arg_t nr)
{
    pthread_t self = __pthread_self();
    if (self->cancelstate & PTHREAD_CANCEL_DISABLE)
        return (__syscall6)(nr, a, b, c, d, e, f);

    long rv = __syscall_cp_asm(a, b, c, d, e, f, nr, &self->cancel);
    if (rv == -EINTR && nr != SYS_close)
        __cancel();
    return rv;
}

hidden void __testcancel(void)
{
    pthread_t self = __pthread_self();
    if (!(self->cancelstate & PTHREAD_CANCEL_DISABLE) && self->cancel)
        __cancel();
}

static void cancel_handler(int sig, siginfo_t *si, void *ctx)
{
    ucontext_t *uc = ctx;
    pthread_t self = __pthread_self();
    if ((self->cancelstate & PTHREAD_CANCEL_DISABLE) || !self->cancel) return;
    if (self->cancelstate & PTHREAD_CANCEL_ASYNCHRONOUS)
        __cancel();
    if (uc->uc_mcontext.REG_IP >= (uintptr_t)__cp_begin && uc->uc_mcontext.REG_IP < (uintptr_t)__cp_end)
        uc->uc_mcontext.REG_IP = (uintptr_t)__cp_cancel;
    BITOP(uc->uc_sigmask.__ss, |=, (SIGCANCEL - 1));
    __syscall(SYS_tkill, self->tid, SIGCANCEL);
}

static void init_cancel_sig(void)
{
    struct sigaction sa = {.sa_sigaction = cancel_handler, .sa_flags = SA_RESTART | SA_SIGINFO};
    memset(sa.sa_mask.__ss, -1, _NSIG / 8);
    __libc_sigaction(SIGCANCEL, &sa, 0);
}

int pthread_cancel(pthread_t t)
{
    static int done;
    if (!done) {
        init_cancel_sig();
        a_swap(&done, 1);
    }
    a_swap(&t->cancel, 1);
    int rv = 0;
    if (t != __pthread_self())
    {
        __lock(&t->killlock);
        if (t->tid)
            rv = -__syscall(SYS_tkill, t->tid, SIGCANCEL);
        __unlock(&t->killlock);
    }
    return rv;
}
