#include <stdio.h>
#include <string.h>
#include <errno.h>

void perror(const char *msg)
{
    const char *s = strerror(errno);
    __FLOCK(stderr);
    if (msg && *msg) {
        fputs(msg, stderr);
        fputs(": ", stderr);
    }
    fputs(s, stderr);
    putc_unlocked('\n', stderr);
    __FUNLOCK(stderr);
}
