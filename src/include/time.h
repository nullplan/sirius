#ifndef TIME_H
#define TIME_H

#include "../../include/time.h"

extern hidden int __clock_getres(clockid_t, struct timespec *);
extern hidden int __clock_gettime(clockid_t, struct timespec *);
struct tz {
    int gmtoff;
    int isdst;
    int oppoff;
    const char *name;
    const char *oppname;
};
extern hidden struct tz __time_to_tz(long long, int);
extern hidden long long __time_to_year(time_t);
extern hidden time_t __year_to_time(long long, int *);
extern hidden int __time_to_tm(struct tm *, time_t, const struct tz *);
#endif
