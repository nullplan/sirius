#include <time.h>

time_t time(time_t *t)
{
    struct timespec ts;
    if (__clock_gettime(CLOCK_REALTIME, &ts)) return -1;
    *t = ts.tv_sec;
    return ts.tv_sec;
}
