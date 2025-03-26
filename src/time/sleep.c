#include <unistd.h>
#include <time.h>

unsigned sleep(unsigned x)
{
    struct timespec ts = {.tv_sec = x};
    if (nanosleep(&ts, &ts))
        return ts.tv_sec;
    return 0;
}
