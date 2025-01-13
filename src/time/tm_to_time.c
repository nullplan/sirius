#include <time.h>

static int month_to_time(int m, int isleap)
{
    static const unsigned short yday_from_mon[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    return yday_from_mon[m] + (isleap && m > 1);
}

hidden time_t __tm_to_time(const struct tm *tm)
{
    long long y = tm->tm_year;
    int m = tm->tm_mon;
    int adj = m / 12;
    m %= 12;
    if (m < 0) {
        m += 12;
        adj--;
    }
    y += adj;

    int isleap;
    time_t rv = __year_to_time(y, &isleap);
    rv += month_to_time(m, isleap);
    rv += tm->mday * 86400ll + tm->tm_hour * 3600ll + tm->tm_min * 60ll + tm->tm_sec;
    return rv;
}
