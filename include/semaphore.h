#ifndef __SEMAPHORE_H
#define __SEMAPHORE_H

#define __NEED_clockid_t
#define __NEED_time_t
#define __NEED_struct_timespec
#include <alltypes.h>
#include <features.h>
#include <fcntl.h>
#ifdef __cplusplus
extern "C" {
#endif

#define SEM_FAILED      ((sem_t *)0)

typedef struct {
    volatile int __ctr;
    volatile int __waiters;
    int __pshared;
} sem_t;

int sem_init(sem_t *, int, unsigned);
int sem_clockwait(sem_t *__restrict, clockid_t, const struct timespec *__restrict);
int sem_getvalue(sem_t *__restrict, int *__restrict);
int sem_post(sem_t *);
int sem_timedwait(sem_t *__restrict, const struct timespec *__restrict);
int sem_trywait(sem_t *);
int sem_wait(sem_t *);
int sem_destroy(sem_t *);

sem_t *sem_open(const char *, int, ...);
int sem_close(sem_t *);
int sem_unlink(const char *);

#ifdef __cplusplus
}
#endif
#endif
