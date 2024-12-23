#ifndef __PTHREAD_H
#define __PTHREAD_H

#define __NEED_struct___cnd
#define __NEED_struct___mtx
#define __NEED_pthread_attr_t
#define __NEED_pthread_barrier_t
#define __NEED_pthread_barrierattr_t
#define __NEED_pthread_cond_t
#define __NEED_pthread_condattr_t
#define __NEED_pthread_key_t
#define __NEED_pthread_mutex_t
#define __NEED_pthread_mutexattr_t
#define __NEED_pthread_once_t
#define __NEED_pthread_rwlock_t
#define __NEED_pthread_rwlockattr_t
#define __NEED_pthread_spinlock_t
#define __NEED_pthread_t
#define __NEED_size_t
#define __NEED_clockid_t
#include <alltypes.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

struct timespec;
#define PTHREAD_NULL                        0
#define PTHREAD_BARRIER_SERIAL_THREAD       1
#define PTHREAD_CANCEL_ASYNCHRONOUS         1
#define PTHREAD_CANCEL_ENABLE               0
#define PTHREAD_CANCEL_DEFERRED             0
#define PTHREAD_CANCEL_DISABLE              1
#define PTHREAD_CANCELED                    ((pthread_t)-1)
#define PTHREAD_CREATE_DETACHED             1
#define PTHREAD_CREATE_JOINABLE             0
#define PTHREAD_EXPLICIT_SCHED              2
#define PTHREAD_INHERIT_SCHED               0
#define PTHREAD_SCOPE_PROCESS               4
#define PTHREAD_SCOPE_SYSTEM                0

#define PTHREAD_MUTEX_DEFAULT               0
#define PTHREAD_MUTEX_ERRORCHECK            1
#define PTHREAD_MUTEX_NORMAL                0
#define PTHREAD_MUTEX_RECURSIVE             2
#define PTHREAD_MUTEX_ROBUST                4
#define PTHREAD_MUTEX_STALLED               0
#define PTHREAD_PRIO_INHERIT                8
#define PTHREAD_PRIO_NONE                   0
#define PTHREAD_PROCESS_SHARED              16
#define PTHREAD_PROCESS_PRIVATE             0

#define PTHREAD_ONCE_INIT                   {0}
#define PTHREAD_COND_INITIALIZER            {0}
#define PTHREAD_MUTEX_INITIALIZER           {0}
#define PTHREAD_RWLOCK_INITIALIZER          {0}

struct sched_param;
int pthread_atfork(void (*)(void), void (*)(void), void (*)(void));
int pthread_attr_init(pthread_attr_t *);
int pthread_attr_getdetachstate(const pthread_attr_t *, int *);
int pthread_attr_getinheritsched(const pthread_attr_t *__restrict, int *__restrict);
int pthread_attr_getschedparam(const pthread_attr_t *__restrict, struct sched_param *__restrict);
int pthread_attr_getschedpolicy(const pthread_attr_t *__restrict, int *__restrict);
int pthread_attr_getscope(const pthread_attr_t *__restrict, int *__restrict);
int pthread_attr_getstack(const pthread_attr_t *__restrict, void **__restrict, size_t *__restrict);
int pthread_attr_getstacksize(const pthread_attr_t *__restrict, size_t *__restrict);
int pthread_attr_getguardsize(const pthread_attr_t *__restrict, size_t *__restrict);
int pthread_attr_setdetachstate(pthread_attr_t *, int);
int pthread_attr_setinheritsched(pthread_attr_t *, int);
int pthread_attr_setschedparam(pthread_attr_t *__restrict, const struct sched_param *__restrict);
int pthread_attr_setschedpolicy(pthread_attr_t *, int);
int pthread_attr_setscope(pthread_attr_t *, int);
int pthread_attr_setstack(pthread_attr_t *, void *, size_t);
int pthread_attr_setstacksize(pthread_attr_t *, size_t);
int pthread_attr_setguardsize(pthread_attr_t *, size_t);
int pthread_attr_destroy(pthread_attr_t *);

int pthread_barrierattr_init(pthread_barrierattr_t *);
int pthread_barrierattr_getpshared(const pthread_barrierattr_t *__restrict, int *__restrict);
int pthread_barrierattr_setpshared(pthread_barrierattr_t *, int);
int pthread_barrierattr_destroy(pthread_barrierattr_t *);

int pthread_barrier_init(pthread_barrier_t *__restrict, const pthread_barrierattr_t *__restrict, unsigned);
int pthread_barrier_wait(pthread_barrier_t *);
int pthread_barrier_destroy(pthread_barrier_t *);

int pthread_condattr_init(pthread_condattr_t *);
int pthread_condattr_getclock(const pthread_condattr_t *__restrict, clockid_t *__restrict);
int pthread_condattr_getpshared(const pthread_condattr_t *__restrict, int *__restrict);
int pthread_condattr_setclock(pthread_condattr_t *, clockid_t);
int pthread_condattr_setpshared(pthread_condattr_t *, int);
int pthread_condattr_destroy(pthread_condattr_t *);

int pthread_cond_init(pthread_cond_t *__restrict, const pthread_condattr_t *__restrict);
int pthread_cond_broadcast(pthread_cond_t *);
int pthread_cond_clockwait(pthread_cond_t *__restrict, pthread_mutex_t *__restrict, clockid_t, const struct timespec *__restrict);
int pthread_cond_signal(pthread_cond_t *);
int pthread_cond_timedwait(pthread_cond_t *__restrict, pthread_mutex_t *__restrict, const struct timespec *__restrict);
int pthread_cond_wait(pthread_cond_t *__restrict, pthread_mutex_t *__restrict);
int pthread_cond_destroy(pthread_cond_t *);

int pthread_cancel(pthread_t);
int pthread_create(pthread_t *__restrict, const pthread_attr_t *__restrict, void *(*)(void*), void *__restrict);
int pthread_detach(pthread_t);
int pthread_equal(pthread_t, pthread_t);
_Noreturn void pthread_exit(void *);
int pthread_join(pthread_t, void **);
int pthread_getcpuclockid(pthread_t, clockid_t *);
int pthread_getschedparam(pthread_t, int *__restrict, struct sched_param *__restrict);
pthread_t pthread_self(void);
int pthread_setcancelstate(int, int *);
int pthread_setcanceltype(int, int *);
int pthread_setschedparam(pthread_t, int, const struct sched_param *);
int pthread_setschedprio(pthread_t, int);
void pthread_testcancel(void);

int pthread_key_create(pthread_key_t *, void (*)(void*));
int pthread_key_delete(pthread_key_t);
void *pthread_getspecific(pthread_key_t);
int pthread_setspecific(pthread_key_t, const void *);

int pthread_mutexattr_init(pthread_mutexattr_t *);
int pthread_mutexattr_getprotocol(const pthread_mutexattr_t *__restrict, int *__restrict);
int pthread_mutexattr_getpshared(const pthread_mutexattr_t *__restrict, int *__restrict);
int pthread_mutexattr_getrobust(const pthread_mutexattr_t *__restrict, int *__restrict);
int pthread_mutexattr_gettype(const pthread_mutexattr_t *__restrict, int *__restrict);
int pthread_mutexattr_setprotocol(pthread_mutexattr_t *, int);
int pthread_mutexattr_setpshared(pthread_mutexattr_t *, int);
int pthread_mutexattr_setrobust(pthread_mutexattr_t *, int);
int pthread_mutexattr_settype(pthread_mutexattr_t *, int);
int pthread_mutexattr_destroy(pthread_mutexattr_t *);

int pthread_mutex_init(pthread_mutex_t *__restrict, const pthread_mutexattr_t *__restrict);
int pthread_mutex_clocklock(pthread_mutex_t *__restrict, clockid_t, const struct timespec *__restrict);
int pthread_mutex_consistent(pthread_mutex_t *);
int pthread_mutex_lock(pthread_mutex_t *);
int pthread_mutex_timedlock(pthread_mutex_t *__restrict, const struct timespec *__restrict);
int pthread_mutex_trylock(pthread_mutex_t *);
int pthread_mutex_unlock(pthread_mutex_t *);
int pthread_mutex_destroy(pthread_mutex_t *);

int pthread_once(pthread_once_t *, void (*)(void));

int pthread_rwlock_init(pthread_rwlock_t *__restrict, const pthread_rwlockattr_t *__restrict);
int pthread_rwlock_clockrdlock(pthread_rwlock_t *__restrict, clockid_t, const struct timespec *__restrict);
int pthread_rwlock_clockwrlock(pthread_rwlock_t *__restrict, clockid_t, const struct timespec *__restrict);
int pthread_rwlock_rdlock(pthread_rwlock_t *);
int pthread_rwlock_timedrdlock(pthread_rwlock_t *__restrict, const struct timespec *__restrict);
int pthread_rwlock_timedwrlock(pthread_rwlock_t *__restrict, const struct timespec *__restrict);
int pthread_rwlock_tryrdlock(pthread_rwlock_t *);
int pthread_rwlock_trywrlock(pthread_rwlock_t *);
int pthread_rwlock_unlock(pthread_rwlock_t *);
int pthread_rwlock_wrlock(pthread_rwlock_t *);
int pthread_rwlock_destroy(pthread_rwlock_t *);

int pthread_rwlockattr_init(pthread_rwlockattr_t *);
int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *__restrict, int *__restrict);
int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *, int);
int pthread_rwlockattr_destroy(pthread_rwlockattr_t *);

int pthread_spin_init(pthread_spinlock_t *, int);
int pthread_spin_lock(pthread_spinlock_t *);
int pthread_spin_trylock(pthread_spinlock_t *);
int pthread_spin_unlock(pthread_spinlock_t *);
int pthread_spin_destroy(pthread_spinlock_t *);

struct __ptcb {
    void (*__fn)(void *);
    void *__a;
    struct __ptcb *__next;
};

void __ptcb_push(struct __ptcb *);
void __ptcb_pop(int);

#define pthread_cleanup_push(fn, a) do {struct __ptcb __cb = {(fn), (a)}; __ptcb_push(&__cb)
#define pthread_cleanup_pop(x) __ptcb_pop(x); } while (0)

#ifdef __cplusplus
}
#endif
#endif
