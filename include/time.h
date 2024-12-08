#ifndef __TEMPLATE_H
#define __TEMPLATE_H

#define __NEED_NULL
#define __NEED_size_t
#define __NEED_time_t
#define __NEED_clock_t
#define __NEED_struct_timespec
#include <alltypes.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

#define CLOCKS_PER_SEC  1000000
#define TIME_UTC        1
#define TIME_MONOTONIC  2
#define TIME_ACTIVE     3
#define TIME_THREAD_ACTIVE 4

struct tm {
    int tm_year;
    int tm_mon;
    int tm_mday;
    int tm_wday;
    int tm_yday;
    int tm_hour;
    int tm_min;
    int tm_sec;
    int tm_isdst;
    int __tm_gmtoff;
    const char *__tm_tzname;
};

clock_t clock(void);
double difftime(time_t, time_t);
time_t mktime(struct tm *);
time_t timegm(struct tm *);
time_t time(time_t *);
__deprecated char *asctime(const struct tm *);
__deprecated char *ctime(const time_t *);
struct tm *gmtime(const time_t *);
struct tm *gmtime_r(const time_t *, struct tm *);
struct tm *localtime(const time_t *);
struct tm *localtime_r(const time_t *, struct tm *);
size_t strftime(char *__restrict, size_t, const char *__restrict, const struct tm *__restrict);
int timespec_get(struct timespec *, int);
int timespec_getres(struct timespec *, int);

#ifdef __cplusplus
}
#endif
#endif
