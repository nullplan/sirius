#include <time.h>
#include <stdio.h>

__deprecated char *asctime(const struct tm *tm)
{
    static char buf[26];
    if (!tm) return 0; /* needed for ctime() */
    static const char wday[] = "SunMonTueWedThuFriSat";
    static const char mon[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
    snprintf(buf, sizeof buf, "%.3s %.3s%3d %.2d:%.2d:%.2d %d\n",
            wday + 3 * tm->tm_wday,
            mon + 3 * tm->tm_mon,
            tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec, 1900 + tm->tm_year);
    return buf;

}
