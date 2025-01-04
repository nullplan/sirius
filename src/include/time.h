#ifndef TIME_H
#define TIME_H

#include "../../include/time.h"

extern hidden int __clock_getres(clockid_t, struct timespec *);
extern hidden int __clock_gettime(clockid_t, struct timespec *);
#endif
