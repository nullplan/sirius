#ifndef __SIGNAL_H
#define __SIGNAL_H

#include <features.h>
#ifdef _POSIX_C_SOURCE
#define __NEED_pthread_t
#define __NEED_size_t
#define __NEED_uid_t
#define __NEED_time_t
#define __NEED_struct_timespec
#define __NEED_pid_t
#define __NEED_pthread_attr_t
#include <alltypes.h>
#endif
#ifdef __cplusplus
extern "C" {
#endif

typedef int sig_atomic_t;

#define SIG_DFL ((void (*)(int))0)
#define SIG_IGN ((void (*)(int))1)
#define SIG_ERR ((void (*)(int))-1)

#define SIGHUP		 1
#define SIGINT		 2
#define SIGQUIT		 3
#define SIGILL		 4
#define SIGTRAP		 5
#define SIGABRT		 6
#define SIGIOT		 6
#define SIGBUS		 7
#define SIGFPE		 8
#define SIGKILL		 9
#define SIGUSR1		10
#define SIGSEGV		11
#define SIGUSR2		12
#define SIGPIPE		13
#define SIGALRM		14
#define SIGTERM		15
#define SIGSTKFLT	16
#define SIGCHLD		17
#define SIGCONT		18
#define SIGSTOP		19
#define SIGTSTP		20
#define SIGTTIN		21
#define SIGTTOU		22
#define SIGURG		23
#define SIGXCPU		24
#define SIGXFSZ		25
#define SIGVTALRM	26
#define SIGPROF		27
#define SIGWINCH	28
#define SIGIO		29
#define SIGPOLL		SIGIO
#define SIGPWR		30
#define SIGSYS		31
int __current_rtmin(void)
#ifdef __GNUC__
    __attribute__((const))
#endif
    ;
int __current_rtmax(void)
#ifdef __GNUC__
    __attribute__((const))
#endif
    ;
#define SIGRTMIN        (__current_rtmin())
#define SIGRTMAX        (__current_rtmax())

#define _NSIG           65

void (*signal(int, void (*)(int)))(int);
int raise(int);

#ifdef _POSIX_C_SOURCE
typedef struct {
    unsigned long __ss[128/sizeof(long)];
} sigset_t;

union sigval {
    int sival_int;
    void *sival_ptr;
};

struct sigevent {
    union sigval sigev_value;
    int sigev_signo;
    int sigev_notify;
    union {
        int _pad[14 - sizeof (union sigval) / sizeof (int)];
        int _tid;
        struct {
            void (*_fn)(union sigval);
            pthread_attr_t *_attr;
        } _s;
    } _u;
};

#define sigev_notify_thread_id  _u._s._tid
#define sigev_notify_function   _u._s._fn
#define sigev_notify_attributes _u._s._attr

#define SIGEV_SIGNAL    0
#define SIGEV_NONE      1
#define SIGEV_THREAD    2
#define SIGEV_THREAD_ID 4

#define SIG2STR_MAX     9

typedef struct {
    void *ss_sp;
    int ss_flags;
    size_t ss_size;
} stack_t;

#ifdef _GNU_SOURCE
#define __NSG(x)    x
#else
#define __NSG(x)    __ ## x
#endif

#include <bits/signal.h>

typedef struct {
    int si_signo, si_errno, si_code;
    union {
        struct {
            pid_t _pid;
            uid_t _uid;
            union sigval _val;
        } _rt;
        struct {
            int _tid;
            int _over;
            union sigval _val;
            int _sys_private;
        } _timer;
        struct {
            pid_t _pid;
            uid_t _uid;
            int _status;
            long _utime;
            long _stime;
        } _child;
        struct {
            void *_addr;
            union {
                int _trapno;
                short _addr_lsb;
                struct {
                    char _dummy[sizeof (void *)];
                    void *_lower;
                    void *_upper;
                } _addr_bnd;
                struct {
                    char _dummy[sizeof (void *)];
                    unsigned _pkey;
                } _addr_pkey;
                struct {
                    unsigned long _data;
                    unsigned _type;
                    unsigned _flags;
                } _perf;
            } _u;
        } _fault;
        struct {
            long _band;
            int _fd;
        } _poll;
        struct {
            void *_call_addr;
            int _syscall;
            unsigned _arch;
        } _sys;
    } _u;
} siginfo_t;

#define si_pid                  _u._rt._pid
#define si_uid                  _u._rt._uid
#define si_value                _u._rt._val
#define si_tid                  _u._timer._tid
#define si_overrun              _u._timer._over
#define si_sys_private          _u._timer._sys_private
#define si_status               _u._child._status
#define si_utime                _u._child._utime
#define si_stime                _u._child._stime
#define si_addr                 _u._fault._addr
#define si_trapno               _u._fault._u._trapno
#define si_addr_lsb             _u._fault._u._addr_lsb
#define si_lower                _u._fault._u._addr_bnd._lower
#define si_upper                _u._fault._u._addr_bnd._upper
#define si_pkey                 _u._fault._u._addr_pkey._pkey
#define si_perf_data            _u._fault._u._perf._data
#define si_perf_type            _u._fault._u._perf._type
#define si_perf_flags           _u._fault._u._perf._flags
#define si_band                 _u._poll._band
#define si_fd                   _u._poll._fd
#define si_call_addr            _u._sys._call_addr
#define si_syscall              _u._sys._syscall
#define si_arch                 _u._sys._arch

#define SI_USER             0
#define SI_KERNEL           0x80
#define SI_QUEUE            (-1)
#define SI_TIMER            (-2)
#define SI_MESGQ            (-3)
#define SI_ASYNCIO          (-4)
#define SI_SIGIO            (-5)
#define SI_TKILL            (-6)
#define SI_DETHREAD         (-7)
#define SI_ASYNCNL          (-60)

#define ILL_ILLOPC          1
#define ILL_ILLOPN          2
#define ILL_ILLADR          3
#define ILL_ILLTRP          4
#define ILL_PRVOPC          5
#define ILL_PRVREG          6
#define ILL_COPROC          7
#define ILL_BADSTK          8

#define FPE_INTDIV          1
#define FPE_INTOVF          2
#define FPE_FLTDIV          3
#define FPE_FLTOVF          4
#define FPE_FLTUND          5
#define FPE_FLTRES          6
#define FPE_FLTINV          7
#define FPE_FLTSUB          8

#define SEGV_MAPERR         1
#define SEGV_ACCERR         2

#define BUS_ADRALN          1
#define BUS_ADRERR          2
#define BUS_OBJERR          3

#define TRAP_BRKPT          1
#define TRAP_TRACE          2

#define CLD_EXITED          1
#define CLD_KILLED          2
#define CLD_DUMPED          3
#define CLD_TRAPPED         4
#define CLD_STOPPED         5
#define CLD_CONTINUED       6

#define SIG_BLOCK           0
#define SIG_UNBLOCK         1
#define SIG_SETMASK         2

#define SA_NOCLDSTOP        0x00000001
#define SA_NOCLDWAIT        0x00000002
#define SA_SIGINFO          0x00000004
#define SA_ONSTACK          0x08000000
#define SA_RESTART          0x10000000
#define SA_NODEFER          0x40000000
#define SA_RESETHAND        0x80000000

#define SS_DISABLE          1
#define SS_ONSTACK          2

struct sigaction {
    union {
        void (*_h)(int);
        void (*_a)(int, siginfo_t *, void *);
    } _u;
    sigset_t sa_mask;
    int sa_flags;
};

#define sa_handler      _u._h
#define sa_sigaction    _u._a

int kill(pid_t, int);
void psiginfo(const siginfo_t *, const char *);
void psignal(int, const char *);
int pthread_kill(pthread_t, int);
int pthread_sigmask(int, const sigset_t *__restrict, sigset_t *__restrict);
int sig2str(int, char *);
int sigaction(int, const struct sigaction *__restrict, struct sigaction *__restrict);
int sigaddset(sigset_t *, int);
int sigdelset(sigset_t *, int);
int sigemptyset(sigset_t *);
int sigfillset(sigset_t *);
int sigismember(const sigset_t *, int);
int sigpending(sigset_t *);
int sigprocmask(int, const sigset_t *__restrict, sigset_t *__restrict);
int sigqueue(pid_t, int, union sigval);
int sigsuspend(const sigset_t *);
int sigtimedwait(const sigset_t *__restrict, siginfo_t *__restrict, const struct timespec *__restrict);
int sigwait(const sigset_t *__restrict, int *__restrict);
int sigwaitinfo(const sigset_t *__restrict, siginfo_t *__restrict);
int str2sig(const char *__restrict, int *__restrict);
#endif

#ifdef _XOPEN_SOURCE
int killpg(pid_t, int);
int sigaltstack(const stack_t *__restrict, stack_t *__restrict);
#endif

#ifdef __cplusplus
}
#endif
#endif
