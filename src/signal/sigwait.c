#include <signal.h>

int sigwait(const sigset_t *restrict ss, int *restrict sig)
{
    siginfo_t si;
    int rv = sigwaitinfo(ss, &si);
    if (!rv) *sig = si.si_signo;
    return rv;
}
