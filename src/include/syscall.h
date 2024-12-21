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

#endif
