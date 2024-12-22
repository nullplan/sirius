#ifndef __SYS_RESOURCE_H
#define __SYS_RESOURCE_H

#include <features.h>
#ifdef _XOPEN_SOURCE
#define __NEED_id_t
#endif
#define __NEED_time_t
#define __NEED_suseconds_t
#define __NEED_struct_timeval
#include <alltypes.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned long long rlim_t;
#define RLIM_INFINITY   (-1ull)
#define RLIM_SAVED_MAX  (-1ull)
#define RLIM_SAVED_CUR  (-1ull)

struct rlimit {
    rlim_t rlim_cur;
    rlim_t rlim_max;
};

#ifdef _GNU_SOURCE
#define __NSG(x)    x
#else
#define __NSG(x)    __##x
#endif

struct rusage {
    _Reg __ru_utime_secs;
    _Reg __ru_utime_usecs;
    _Reg __ru_stime_secs;
    _Reg __ru_stime_usecs;
    _Reg ru_maxrss;
    _Reg ru_ixrss;
    _Reg ru_idrss;
    _Reg ru_isrss;
    _Reg ru_minflt;
    _Reg ru_majflt;
    _Reg ru_nswap;
    _Reg ru_inblock;
    _Reg ru_oublock;
    _Reg ru_msgsnd;
    _Reg ru_msgrcv;
    _Reg ru_nsignals;
    _Reg ru_nvcsw;
    _Reg ru_nivcsw;

    struct timeval ru_utime;
    struct timeval ru_stime;
};

#define RLIMIT_CPU      0
#define RLIMIT_FSIZE    1
#define RLIMIT_DATA     2
#define RLIMIT_STACK    3
#define RLIMIT_CORE     4
#define RLIMIT_RSS      5
#define RLIMIT_NPROC    6
#define RLIMIT_NOFILE   7
#define RLIMIT_MEMLOCK  8
#define RLIMIT_AS       9
#define RLIMIT_LOCKS    10
#define RLIMIT_SIGPENDING   11
#define RLIMIT_MSGQUEUE 12
#define RLIMIT_NICE     13
#define RLIMIT_RTPRIO   14
#define RLIMIT_RTTIME   15

int getrlimit(int, struct rlimit *);
int setrlimit(int, const struct rlimit *);

#ifdef _XOPEN_SOURCE
#define PRIO_PROCESS    0
#define PRIO_PGRP       1
#define PRIO_USER       2

#define RUSAGE_SELF     0
#define RUSAGE_CHILDREN (-1)
#define RUSAGE_BOTH     (-2)
#define RUSAGE_THREAD   1

int getpriority(int, id_t);
int getrusage(int, struct rusage *);
int setpriority(int, id_t, int);
#endif
#ifdef __cplusplus
}
#endif
#endif
