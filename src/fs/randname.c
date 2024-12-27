#include <unistd.h>
#include <time.h>
#include <pthread.h>

hidden void __randname(char *p)
{
    struct timespec ts;
    unsigned entropy = __pthread_self()->tid;
    if (timespec_get(&ts, TIME_MONOTONIC))
        entropy ^= ts.tv_sec ^ ts.tv_nsec;
    for (int i = 0; i < 6; i++, p++, entropy >>= 5) {
        *p = 'A' + (entropy & 15);
        if (entropy & 16) *p += 32;
    }
}
