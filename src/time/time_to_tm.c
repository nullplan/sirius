#include <time.h>
#include <stdint.h>
#include <errno.h>

/* Turns out the year being 32 bits is the bigger limitation
 * than the calculations in __year_to_time().
 *
 * 365*86400 is a 25 bit number. tm_year is a 31 bit number.
 * Therefore we just reject all input times outside of ±2^56.
 */

hidden int __time_to_tm(struct tm *tm, time_t t, const struct tz *tz)
{
    if (t + (1ull << 56) >= 1ull << 57) {
        errno = EOVERFLOW;
        return -1;
    }
    int y = __time_to_year(t);
    int is_leap;
    int second_in_year = t - __year_to_time(y, &is_leap);
    tm->tm_year = y;

    static const unsigned short month_start[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
    int d = second_in_year / 86400;
    int second = second_in_year % 86400;
    if (second < 0) {
        d--;
        second += 86400;
    }
    tm->tm_yday = d;
    if (is_leap && d == 60) {
        tm->tm_mon = 1;
        tm->tm_mday = 29;
    } else {
        if (is_leap && d > 60) d--;
        for (tm->tm_mon = 0; month_start[tm->tm_mon+1] < d; tm->tm_mon++);
        tm->tm_mday = d - month_start[tm->tm_mon] + 1;
    }
    tm->tm_hour = second / 3600;
    tm->tm_min = (second / 60) % 60;
    tm->tm_sec = second % 60;
    tm->tm_wday = (t / 86400 + 3) % 7;
    if (tm->tm_wday < 0) tm->tm_wday += 7;
    tm->tm_isdst = tz->isdst;
    tm->__tm_gmtoff = tz->gmtoff;
    tm->__tm_tzname = tz->name;
    return 0;
}
