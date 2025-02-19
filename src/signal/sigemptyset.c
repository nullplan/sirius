#include <signal.h>
#include <string.h>

int sigemptyset(sigset_t *ss)
{
    memset(ss, 0, _NSIG/8);
    return 0;
}
