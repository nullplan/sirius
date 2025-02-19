#include <signal.h>

int killpg(pid_t pid, int sig)
{
    return kill(-pid, sig);
}
