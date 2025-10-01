#include <time.h>

struct tm *localtime_r(const time_t *t, struct tm *tm)
{
    struct tz ltz = __time_to_tz(*t, 0);
    if (__time_to_tm(tm, *t + ltz.gmtoff, &ltz))
        return 0;
    return tm;
}
