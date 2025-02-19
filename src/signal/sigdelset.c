#include <signal.h>
#include <errno.h>
#include "libc.h"

int sigdelset(sigset_t *ss, int sig)
{
    if (sig - 1u >= _NSIG - 1u || sig - 32u < 3u) {
        errno = EINVAL;
        return -1;
    }
    BITOP(ss->__ss, &=~, sig-1);
    return 0;
}
