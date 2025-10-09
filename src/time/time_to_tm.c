#include <time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>

static int days_in_month(int m, int isleap)
{
    /* long month mask:
     * JFMAMJJASOND
     * 101010110101
     *
     * reversed and grouped into fours:
     * 1010 1101 0101
     */
    if (m == 1) return 28 + isleap;
    return 30 + ((0xad5 >> m) & 1);
}

/* Turns out the year being 32 bits is the bigger limitation
 * than the calculations in __year_to_time().
 *
 * 365*86400 is a 25 bit number. tm_year is a 31 bit number.
 * Therefore we just reject all input times outside of ±2^55.
 */

hidden int __time_to_tm(struct tm *tm, time_t t, const struct tz *tz)
{
    if (t + (1ull << 55) >= 1ull << 56) {
        errno = EOVERFLOW;
        return -1;
    }
    int y = __time_to_year(t);
    int is_leap;
    int second_in_year = t - __year_to_time(y, &is_leap);
    tm->tm_year = y;

    int d = second_in_year / 86400;
    int second = second_in_year % 86400;
    assert(second >= 0);
    tm->tm_yday = d;
    for (tm->tm_mon = 0; d >= days_in_month(tm->tm_mon, is_leap); d -= days_in_month(tm->tm_mon, is_leap), tm->tm_mon++);
    tm->tm_mday = d + 1;
    tm->tm_hour = second / 3600;
    tm->tm_min = (second / 60) % 60;
    tm->tm_sec = second % 60;
    tm->tm_wday = (t / 86400 + 4) % 7;
    if (tm->tm_wday < 0) tm->tm_wday += 7;
    tm->tm_isdst = tz->isdst;
    tm->__tm_gmtoff = tz->gmtoff;
    tm->__tm_tzname = tz->name;
    return 0;
}
