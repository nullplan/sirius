#include <time.h>

time_t mktime(struct tm *tm)
{
    time_t rv = __tm_to_time(tm);
    struct tz tz = __time_to_tz(rv, 1);
    if (tm->tm_isdst < 0 || !tm->tm_isdst == !tz->isdst)
        rv -= tm->gmtoff;
    else
        rv -= tm->oppoff;
    tz = __time_to_tz(rv, 0);
    __time_to_tm(tm, rv, tz);
    return rv;
}
