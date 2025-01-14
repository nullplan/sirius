#include <time.h>

time_t mktime(struct tm *tm)
{
    time_t local = __tm_to_time(tm);
    struct tz tz;
    tz = __time_to_tz(local, 1);
    if (tm->tm_isdst >= 0 && !tm->tm_isdst != !tz.isdst) {
        if (!tm->tm_isdst)
            tz = __get_std_tz();
        else
            tz = __get_dst_tz();
    }
    __time_to_tm(tm, local, &tz);
    return local + tz.gmtoff;
}
