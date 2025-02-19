#include <signal.h>
#include <errno.h>

int sigprocmask(int act, const sigset_t *restrict ss, sigset_t *restrict oss)
{
    int rv = pthread_sigmask(act, ss, oss);
    if (rv) {
        errno = rv;
        rv = -1;
    }
    return rv;
}
