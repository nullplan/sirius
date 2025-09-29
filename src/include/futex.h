#ifndef FUTEX_H
#define FUTEX_H

#include <features.h>

#define FUTEX_WAIT          0
#define FUTEX_WAKE          1
#define FUTEX_LOCK_PI       6
#define FUTEX_UNLOCK_PI     7
#define FUTEX_TRYLOCK_PI    8
#define FUTEX_LOCK_PI2      13
#define FUTEX_PRIVATE_FLAG  128

struct timespec;

extern hidden int __futex_wait(volatile int *, int, int);
extern hidden int __futex_wait_cp(volatile int *, int, int);
extern hidden int __futex_wake(volatile int *, int, int);
extern hidden int __timedwait(volatile int *, int, int, const struct timespec *, int);
extern hidden int __timedwait_cp(volatile int *, int, int, const struct timespec *, int);

#endif
