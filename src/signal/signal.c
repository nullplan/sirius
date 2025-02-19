#include <signal.h>
#include <string.h>

void (*signal(int sig, void (*fct)(int)))(int)
{
    struct sigaction sa, osa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = fct;
    int rv = __sigaction(sig, &sa, &osa);
    if (rv) return SIG_ERR;
    return osa.sa_handler;
}
