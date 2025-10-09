#include <time.h>
/* taking all rules of the Gregorian calendar together,
 * the year has on average 365.2425 days.
 * 365.2425 * 86400 = 31,556,952
 */
hidden long long __time_to_year(time_t t)
{
    long long y = t / 31556952 + 70;
    int mod = t % 31556952;
    if (mod < 0) y--;
    while (__year_to_time(y, 0) > t) y--;
    while (__year_to_time(y+1, 0) <= t) y++;
    return y;
}
