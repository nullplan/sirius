#include <time.h>

time_t mktime(struct tm *tm)
{
    time_t local = __tm_to_time(tm);
    struct tz tz;
    tz = __time_to_tz(local, 1);
    if (tm->tm_isdst >= 0 && !!tm->tm_isdst != tz.isdst) {
        struct tz wanted_tz;
        if (!tm->tm_isdst)
            wanted_tz = __get_std_tz();
        else
            wanted_tz = __get_dst_tz();
        local += wanted_tz.gmtoff - tz.gmtoff;
        tz = __time_to_tz(local, 1);
    }
    if (__time_to_tm(tm, local, &tz))
        return -1;
    return local - tz.gmtoff;
}
