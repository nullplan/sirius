#include <signal.h>
#include <stdio.h>
#include <string.h>

void psignal(int sig, const char *msg)
{
    const char *sigstr = strsignal(sig);
    fprintf(stderr, "%s%s%s\n", msg? msg : "", msg && *msg? ": " : "", sigstr);
}
