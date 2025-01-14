#ifndef TIME_H
#define TIME_H

#include "../../include/time.h"

extern hidden int __clock_getres(clockid_t, struct timespec *);
extern hidden int __clock_gettime(clockid_t, struct timespec *);
struct tz {
    int gmtoff;
    int isdst;
    const char *name;
};
extern hidden struct tz __time_to_tz(long long, int);
extern hidden struct tz __get_std_tz(void);
extern hidden struct tz __get_dst_tz(void);
extern hidden long long __time_to_year(time_t);
extern hidden time_t __year_to_time(long long, int *);
extern hidden int __time_to_tm(struct tm *, time_t, const struct tz *);
extern hidden time_t __tm_to_time(const struct tm *);
#endif
