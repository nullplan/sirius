#ifndef __TEMPLATE_H
#define __TEMPLATE_H

#include <features.h>
#define __NEED_NULL
#define __NEED_size_t
#define __NEED_time_t
#define __NEED_clock_t
#define __NEED_struct_timespec
#ifdef _POSIX_C_SOURCE
#define __NEED_clockid_t
#define __NEED_timer_t
#define __NEED_locale_t
#define __NEED_pid_t
#endif
#include <alltypes.h>
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

#ifdef _POSIX_C_SOURCE
struct sigevent;
struct itimerspec {
    struct timespec it_interval;
    struct timespec it_value;
};

#define ITIMER_REAL     0
#define ITIMER_VIRTUAL  1
#define ITIMER_PROF     2

#define CLOCK_REALTIME              0
#define CLOCK_MONOTONIC             1
#define CLOCK_PROCESS_CPUTIME_ID    2
#define CLOCK_THREAD_CPUTIME_ID     3
#define CLOCK_MONOTONIC_RAW         4
#define CLOCK_REALTIME_COARSE       5
#define CLOCK_MONOTONIC_COARSE      6
#define CLOCK_BOOTTIME              7
#define CLOCK_REALTIME_ALARM        8
#define CLOCK_BOOTTIME_ALARM        9
#define CLOCK_TAI                   11

#define TIMER_ABSTIME               0x01

int clock_getcpuclockid(pid_t, clockid_t *);
int clock_getres(clockid_t, struct timespec *);
int clock_gettime(clockid_t, struct timespec *);
int clock_nanosleep(clockid_t, int, const struct timespec *, struct timespec *);
int clock_settime(clockid_t, const struct timespec *);
int nanosleep(const struct timespec *, struct timespec *);
size_t strftime_l(char *__restrict, size_t, const char *__restrict, const struct tm *__restrict, locale_t);
int timer_create(clockid_t, struct sigevent *__restrict, timer_t *__restrict);
int timer_delete(timer_t);
int timer_getoverrun(timer_t);
int timer_gettime(timer_t, struct itimerspec *);
int timer_settime(timer_t, int, const struct itimerspec *__restrict, struct itimerspec *__restrict);
void tzset(void);
extern char *tzname[];
#endif

#ifdef _XOPEN_SOURCE
int __getdate_err(void);
#define getdate_err (__getdate_err())
struct tm *getdate(const char *);
char *strptime(const char *__restrict, const char *__restrict, struct tm *__restrict);
extern int daylight;
extern long timezone;
#endif

#ifdef __cplusplus
}
#endif
#endif
