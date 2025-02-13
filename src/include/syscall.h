#ifndef SYSCALL_H
#define SYSCALL_H

#include <errno.h>
#include "syscall_arch.h"
#include <bits/syscalls.h>
static inline long __syscall_ret(long rv)
{
    if (rv > -4096UL) {
        errno = -rv;
        rv = -1;
    }
    return rv;
}
#define __SYSCALL_NARGS2(nr, a, b, c, d, e, f, n, ...) n
#define __SYSCALL_NARGS(...) __SYSCALL_NARGS2(__VA_ARGS__, 6, 5, 4, 3, 2, 1, 0)
#define __SYSCALL_CONCAT2(a, b) a##b
#define __SYSCALL_CONCAT(a, b) __SYSCALL_CONCAT2(a, b)
#define __syscall(...)  __SYSCALL_CONCAT(__syscall, __SYSCALL_NARGS(__VA_ARGS__))(__VA_ARGS__)
#define syscall(...) __syscall_ret(__syscall(__VA_ARGS__))

#ifndef __scc
typedef long syscall_arg_t;
#define __scc(x) ((long)(x))
#endif

#ifndef __SYSCALL_LL_E
_Static_assert(sizeof (syscall_arg_t) == sizeof (long long), "");
#define __SYSCALL_LL_E(x) x
#define __SYSCALL_LL_O(x) x
#endif

#define __syscall1(nr, a)                       __syscall1((nr), __scc(a))
#define __syscall2(nr, a, b)                    __syscall2((nr), __scc(a), __scc(b))
#define __syscall3(nr, a, b, c)                 __syscall3((nr), __scc(a), __scc(b), __scc(c))
#define __syscall4(nr, a, b, c, d)              __syscall4((nr), __scc(a), __scc(b), __scc(c), __scc(d))
#define __syscall5(nr, a, b, c, d, e)           __syscall5((nr), __scc(a), __scc(b), __scc(c), __scc(d), __scc(e))
#define __syscall6(nr, a, b, c, d, e, f)        __syscall6((nr), __scc(a), __scc(b), __scc(c), __scc(d), __scc(e), __scc(f))

extern hidden long __syscall_cp(syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t);
extern hidden long __syscall_cp_c(syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t, syscall_arg_t);
#define __syscall_cp(...)       __SYSCALL_CONCAT(__syscall_cp, __SYSCALL_NARGS(__VA_ARGS__))(__VA_ARGS__)
#define syscall_cp(...)         __syscall_ret(__syscall_cp(__VA_ARGS__))

#define __syscall_cp0(nr)                       (__syscall_cp)(0, 0, 0, 0, 0, 0, (nr))
#define __syscall_cp1(nr, a)                    (__syscall_cp)(__scc(a), 0, 0, 0, 0, 0, (nr))
#define __syscall_cp2(nr, a, b)                 (__syscall_cp)(__scc(a), __scc(b), 0, 0, 0, 0, (nr))
#define __syscall_cp3(nr, a, b, c)              (__syscall_cp)(__scc(a), __scc(b), __scc(c), 0, 0, 0, (nr))
#define __syscall_cp4(nr, a, b, c, d)           (__syscall_cp)(__scc(a), __scc(b), __scc(c), __scc(d), 0, 0, (nr))
#define __syscall_cp5(nr, a, b, c, d, f)        (__syscall_cp)(__scc(a), __scc(b), __scc(c), __scc(d), __scc(e), 0, (nr))
#define __syscall_cp6(nr, a, b, c, d, e, f)     (__syscall_cp)(__scc(a), __scc(b), __scc(c), __scc(d), __scc(e), __scc(f), (nr))
#ifdef SYS_open
#define __sys_open(nm, ...) __syscall(SYS_open, (nm), O_LARGEFILE | __VA_ARGS__)
#else
#define __sys_open(nm, ...) __syscall(SYS_openat, -100, (nm), O_LARGEFILE | __VA_ARGS__)
#endif

/* socket call constants */
#define SYS_SOCKET              1
#define SYS_BIND                2
#define SYS_CONNECT             3
#define SYS_LISTEN              4
#define SYS_ACCEPT              5
#define SYS_GETSOCKNAME         6
#define SYS_GETPEERNAME         7
#define SYS_SOCKETPAIR          8
#define SYS_SEND                9
#define SYS_RECV                10
#define SYS_SENDTO              11
#define SYS_RECVFROM            12
#define SYS_SHUTDOWN            13
#define SYS_SETSOCKOPT          14
#define SYS_GETSOCKOPT          15
#define SYS_SENDMSG             16
#define SYS_RECVMSG             17
#define SYS_ACCEPT4             18
#define SYS_RECVMMSG            19
#define SYS_SENDMMSG            20
#endif
