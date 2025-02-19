#include <signal.h>
#include "libc.h"

int sigismember(const sigset_t *ss, int sig)
{
    if (sig - 1u >= _NSIG - 1u || sig - 32u < 3u) {
        return 0;
    }
    return !!(BITOP(ss->__ss, &, sig-1));
}
