#include <time.h>

time_t time(time_t *t)
{
    struct timespec ts;
    if (__clock_gettime(CLOCK_REALTIME, &ts)) return -1;
    if (t) *t = ts.tv_sec;
    return ts.tv_sec;
}
