#ifndef __SYS_TIMES_H
#define __SYS_TIMES_H

#define __NEED_clock_t
#include <alltypes.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

struct tms {
    clock_t tms_utime;
    clock_t tms_stime;
    clock_t tms_cutime;
    clock_t tms_cstime;
};

clock_t times(struct tms *);

#ifdef __cplusplus
}
#endif
#endif
