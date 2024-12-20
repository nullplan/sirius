#ifndef __SCHED_H
#define __SCHED_H

#define __NEED_pid_t
#define __NEED_time_t
#define __NEED_struct_timespec
#include <alltypes.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

struct sched_param {
    int sched_priority;
};

#define SCHED_NORMAL    0
#define SCHED_FIFO      1
#define SCHED_RR        2
#define SCHED_BATCH     3
#define SCHED_IDLE      5
#define SCHED_DEADLINE  6
#define SCHED_EXT       7

#define SCHED_OTHER     SCHED_IDLE  /* ??? */

int sched_get_priority_min(int);
int sched_get_priority_max(int);
int sched_get_rr_interval(pid_t, struct timespec *);
int sched_yield(void);

#ifdef __cplusplus
}
#endif
#endif
