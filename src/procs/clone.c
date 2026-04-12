#include <sched.h>
#include <stdarg.h>
#include <pthread.h>
#include "syscall.h"

#define CLONE_VM                0x00000100
#define CLONE_FS                0x00000200
#define CLONE_FILES             0x00000400
#define CLONE_SIGHAND           0x00000800
#define CLONE_PIDFD             0x00001000
#define CLONE_PTRACE            0x00002000
#define CLONE_VFORK             0x00004000
#define CLONE_PARENT            0x00008000
#define CLONE_THREAD            0x00010000
#define CLONE_NEWNS             0x00020000
#define CLONE_SYSVSEM           0x00040000
#define CLONE_SETTLS            0x00080000
#define CLONE_PARENT_SETTID     0x00100000
#define CLONE_CHILD_CLEARTID    0x00200000
#define CLONE_DETACHED          0x00400000
#define CLONE_UNTRACED          0x00800000
#define CLONE_CHILD_SETTID      0x01000000
#define CLONE_NEWCGROUP         0x02000000
#define CLONE_NEWUTS            0x04000000
#define CLONE_NEWIPC            0x08000000
#define CLONE_NEWUSER           0x10000000
#define CLONE_NEWPID            0x20000000
#define CLONE_NEWNET            0x40000000
#define CLONE_IO                0x80000000

int clone(int (*fn)(void *), void *stack, int flags, void *arg, ...)
{
    pid_t *ptid = 0, *ctid = 0;
    uintptr_t tls = 0;

    if ((flags & (CLONE_CHILD_CLEARTID | CLONE_THREAD | CLONE_SETTLS))  /* flags that create an invalid thread state */
            || ((flags & CLONE_VM) && !stack)                           /* can't share stacks */
            || (fn && !stack)                                           /* need either both fn and stack or neither */
            || (!fn && stack))
        return __syscall_ret(-EINVAL);

    va_list ap;
    va_start(ap, arg);
    if (flags & (CLONE_PARENT_SETTID | CLONE_PIDFD | CLONE_CHILD_SETTID))
        ptid = va_arg(ap, void *);
    if (flags & CLONE_CHILD_SETTID) {
        tls = va_arg(ap, uintptr_t);
        ctid = va_arg(ap, void *);
    }
    va_end(ap);
    return __syscall_ret(__clone(fn, stack, flags, arg, ptid, tls, ctid));
}
