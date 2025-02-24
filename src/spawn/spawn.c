#define _GNU_SOURCE
#include <spawn.h>
#include <sched.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include "libc.h"
#include "syscall.h"

struct args {
    const char *name;
    char *const *args;
    char *const *env;
    const struct __fa *fa;
    posix_spawnattr_t psa;
    int (*exec)(const char *, char *const *, char *const *);
    int p[2];
};

static int is_sig_ignored(int sig)
{
    struct sigaction sa;
    __libc_sigaction(sig, 0, &sa);
    return sa.sa_handler == SIG_IGN;
}
static void set_handler(int sig, void (*handler)(int))
{
    struct sigaction sa = {.sa_handler = handler};
    __libc_sigaction(sig, &sa, 0);
}

static int child(void *args)
{
    struct args *a = args;
    int pfd = a->p[1];
    int rv;
    unsigned long handler_set[_NSIG/8/sizeof (long)];
    __get_handler_set(handler_set);
    __syscall(SYS_close, a->p[0]);

    for (const struct __fa *fa = a->fa; fa; fa = fa->next)
    {
        switch (fa->fdop) {
            case FDOP_OPEN:
                if (pfd == fa->fd) {
                    rv = fcntl(pfd, F_DUPFD_CLOEXEC, 0);
                    if (rv == -1) goto out_errno;
                    pfd = rv;
                }
                rv = __sys_open(fa->name, fa->fd2, fa->mode);
                if (rv < 0) goto out_rv;
                if (rv != fa->fd) {
                    #ifdef SYS_dup2
                    rv = __syscall(SYS_dup2, rv, fa->fd);
                    #else
                    rv = __syscall(SYS_dup3, rv, fa->fd, 0);
                    #endif
                    if (rv < 0) goto out_rv;
                }
                break;

            case FDOP_CLOSE:
                if (pfd == fa->fd) {
                    rv = -EBADF;
                    goto out_rv;
                }
                rv = __syscall(SYS_close, fa->fd);
                if (rv) goto out_rv;
                break;

            case FDOP_DUP2:
                if (pfd == fa->fd) {
                    rv = -EBADF;
                    goto out_rv;
                }
                if (pfd == fa->fd2) {
                    rv = fcntl(pfd, F_DUPFD_CLOEXEC, MAX(fa->fd, fa->fd2) + 1);
                    if (rv == -1) goto out_errno;
                    pfd = rv;
                }
                #ifdef SYS_dup2
                rv = __syscall(SYS_dup2, fa->fd, fa->fd2);
                #else
                rv = __syscall(SYS_dup3, fa->fd, fa->fd2, 0);
                #endif
                if (rv < 0) goto out_rv;
                break;

            case FDOP_CHDIR:
                rv = __syscall(SYS_chdir, fa->name);
                if (rv) goto out_rv;
                break;

            case FDOP_FCHDIR:
                if (fa->fd == pfd) {
                    rv = -EBADF;
                    goto out_rv;
                }
                rv = __syscall(SYS_fchdir, fa->fd);
                if (rv) goto out_rv;
                break;
        }
    }

    /* need to go through all signals.
     * Handling must be set to SIG_DFL or SIG_IGN before unblocking
     * signals.
     * SIGTSTP is special. It is the only signal that is routinely sent to
     * a process group, causes stop by default, and can be blocked.
     * It is possible for this process to catch and hold onto a SIGTSTP,
     * then switch process groups and unblock signals, leading to a stopped
     * process that cannot be reactivated the same way it was stopped.
     * For this reason, if SIGTSTP is to be set to default, I will first
     * set it to ignore, then unblock signals, and then set it to default.
     */
    int tstp_default = 0;
    for (int sig = 1; sig < _NSIG; sig++)
    {
        if (sig == SIGTSTP)
        {
            if (!is_sig_ignored(sig))
            {
                set_handler(sig, SIG_IGN);
                tstp_default = 1;
            }
        } else if (sig - 32u < 3u)
            set_handler(sig, SIG_IGN);
        else if (sigismember((sigset_t *)a->psa.__dfl, sig)
                || (sigismember((sigset_t *)handler_set, sig) && !is_sig_ignored(sig)))
            set_handler(sig, SIG_DFL);
    }
    if (a->psa.__flg & POSIX_SPAWN_SETSID) {
        rv =__syscall(SYS_setsid);
        if (rv < 0) goto out_rv;
    } else if (a->psa.__flg & POSIX_SPAWN_SETPGROUP) {
        rv = __syscall(SYS_setpgid, a->psa.__pgrp);
        if (rv < 0) goto out_rv;
    }
    if (a->psa.__flg & POSIX_SPAWN_RESETIDS) {
        __syscall(SYS_setgid, __syscall(SYS_getgid));
        __syscall(SYS_setuid, __syscall(SYS_getuid));
    }
    __syscall(SYS_rt_sigprocmask, SIG_SETMASK, a->psa.__msk, 0, _NSIG/8);
    if (tstp_default)
        set_handler(SIGTSTP, SIG_DFL);

    a->exec(a->name, a->args, a->env);

out_errno:
    rv = -errno;
out_rv:
    while (__syscall(SYS_write, pfd, &rv, sizeof (int)) == -EINTR);
    return 127;
}

hidden int __spawn(pid_t *restrict ppid, const char *restrict name,
        const posix_spawn_file_actions_t *restrict psfa,
        const posix_spawnattr_t *restrict psa,
        char *const args[restrict], char *const env[restrict],
        int (*exec)(const char *, char *const *, char *const *))
{
    struct args a;
    sigset_t oldmask;
    sigset_t *pmask = &oldmask;
    union {
        unsigned long align;
        char buf[PATH_MAX+1024];
    } stack;
    a.name = name;
    a.args = args;
    a.env = env;
    a.fa = psfa? psfa->__f : 0;
    a.psa = psa? *psa : (posix_spawnattr_t){0};
    a.exec = exec;
    if (!(a.psa.__flg & POSIX_SPAWN_SETSIGDEF))
        memset(a.psa.__dfl, 0, _NSIG/8);
    if (!(a.psa.__flg & POSIX_SPAWN_SETSIGMASK))
        pmask = (sigset_t *)a.psa.__msk;

    int rv = pipe2(a.p, O_CLOEXEC);
    if (rv == -1)
        return errno;
    __block_all_sigs(pmask);
    pid_t pid = __clone(child, stack.buf + sizeof stack, SIGCHLD | CLONE_VM,
            &a, 0, 0, 0);
    __restore_sigs(pmask);
    __syscall(SYS_close, a.p[1]);
    if (pid >= 0) {
        int rd;
        do rd = __syscall(SYS_read, a.p[0], &rv, sizeof (int));
        while (rd == -EINTR);
        if (rd == sizeof (int)) {
            int wret;
            do wret = waitpid(pid, 0, 0);
            while (wret != pid || errno != ECHILD);
        } else if (ppid) *ppid = pid;
    }
    else
        rv = pid;
    __syscall(SYS_close, a.p[0]);
    return -rv;
}
