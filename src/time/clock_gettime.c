#include <time.h>
#include "syscall.h"
#include "vdso.h"
#include "cpu.h"

static int cgt_syscall(clockid_t clk, struct timespec *ts)
{
    return __syscall(SYS_clock_gettime, clk, ts);
}

static int cgt_dispatch(clockid_t, struct timespec *);
static void *impl = (void *)cgt_dispatch;

static int cgt_dispatch(clockid_t clk, struct timespec *ts)
{
    void *x = 0;
    /* On ARM32, it is not always possible to provide a userspace CGT.
     * Linux detects the condition when the CPU can't do it, and disables
     * the VDSO CGT in this case so __vdsosym() will return 0.
     * However, being a 32-bit architecture, there are two versions of
     * CGT in the VDSO, and in some kernel versions, Linux forgot to
     * disable the 64-bit version, which is the one we use exclusively.
     * So if we ever port that platform, we must enable the following code there.
     */
#ifdef VDSO_CGT_WORKAROUND
    if (__vdsosym(VDSO_CGT32_NAME, VDSO_CGT32_VER))
#endif
        x = __vdsosym(VDSO_CGT_NAME, VDSO_CGT_VER);

    if (!x)
        x = (void *)cgt_syscall;

    a_cas_p(&impl, (void *)cgt_dispatch, x);
    return ((int (*)(clockid_t, struct timespec *))x)(clk, ts);
}

hidden int __clock_gettime(clockid_t clk, struct timespec *ts)
{
    int (*fct)(clockid_t, struct timespec *) = impl;
    return __syscall_ret(fct(clk, ts));
}
weak_alias(clock_gettime, __clock_gettime);

