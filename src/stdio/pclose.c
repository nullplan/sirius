#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>

int pclose(FILE *f)
{
    int stat = -1;
    int pid = f->popen_pid;
    int rv;
    fclose(f);

    while ((rv = waitpid(pid, &stat, 0)) < 0 && errno == EINTR);
    if (!rv) rv = stat;
    return rv;
}
