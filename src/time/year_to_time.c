#include <time.h>

/* Gregorian calendar definitions:
 * A cycle is a 400 year period starting on January 1st of a year one after a year divisible by 400 (e.g. 1601 or 2001).
 * A century is a 100 year period starting on January 1st of a year one after a year divisible by 100.
 * A group is a 4 year period starting after a year divisible by 4.
 *
 * There are 365 days in a normal year.
 * There are 366 days in a leap year. A leap year is the last year of a group unless it is the last year of a normal century.
 * There are therefore 1461 days in a group.
 * There are 36524 days in a normal century.
 * There are 36525 days in a leap century. A leap century is the last century of a cycle.
 * There are 146097 days per cycle.
 *
 * My algorithm uses 2001 as basis. There are exactly 11323 days between 1970 and 2001.
 */
hidden time_t __year_to_time(long long y, int *is_leap)
{
    int dummy;
    if (!is_leap) is_leap = &dummy;
    /* in the interval [1902;2038], not only can we use a simplified
     * leap year rule, but also the calculation is entirely possible
     * in 32 bits (which is a performance boost on 32-bit archs).
     */
    if (y-2ull <= 136ull) {
        int years_since_1970 = y - 70;
        int leaps = (years_since_1970 + 2) / 4;
        int mod = (years_since_1970 + 2) % 4;
        if (mod <= 0) leaps--;
        *is_leap = !mod;
        return (years_since_1970 * 365 + leaps) * 86400;
    }
    y -= 101; /* now years since 2001 */

    long long cycle = y / 400;
    int year_in_cycle = y % 400;
    if (year_in_cycle < 0) {
        year_in_cycle += 400;
        cycle--;
    }
    int century = year_in_cycle / 100;
    int year_in_century = year_in_cycle % 100;
    int group = year_in_century / 4;
    int year_in_group = year_in_century % 4;
    *is_leap = year_in_group == 3 && (group != 24 || century == 3);
    return (146097ll * cycle + 36524 * century + 1461 * group + 365 * year_in_group + 11323) * 86400;
}
