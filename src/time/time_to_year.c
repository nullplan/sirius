#include <time.h>
hidden long long __time_to_year(time_t t)
{
    long long y = t / 31556952 + 70;
    int mod = t % 31556952;
    if (mod < 0) y--;
    while (__year_to_time(y, 0) > t) y--;
    while (__year_to_time(y+1, 0) <= t) y++;
    return y;
}
