#include <time.h>

struct tm *gmtime_r(const time_t *t, struct tm *tm)
{
    struct tz gmt = {0,0,"UTC"};
    if (__time_to_tm(tm, *t, &gmt))
        return 0;
    return tm;
}
