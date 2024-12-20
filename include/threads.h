#ifndef __THREADS_H
#define __THREADS_H

#define __NEED_struct___cnd
#define __NEED_struct___mtx
#include <alltypes.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

#define thread_local _Thread_local
#define ONCE_FLAG_INIT  {0}
#define TSS_DTOR_ITERATIONS     8
struct __pthread;
struct timespec;
typedef struct __pthread *thrd_t;
typedef struct __cnd cnd_t;
typedef struct __mtx mtx_t;
typedef struct { int __k; } tss_t;
typedef void (*tss_dtor_t)(void *);
typedef int (*thrd_start_t)(void *);
typedef struct { volatile int __ctrl; } once_flag;

enum {
    mtx_plain = 0,
    mtx_recursive = 1,
    mtx_timed = 2,
    thrd_timedout = -2,
    thrd_success = 0,
    thrd_busy = -3,
    thrd_error = -1,
    thrd_nomem = -4,
};

void call_once(once_flag *, void (*)(void));
int cnd_broadcast(cnd_t *);
void cnd_destroy(cnd_t *);
int cnd_init(cnd_t *);
int cnd_signal(cnd_t *);
int cnd_timedwait(cnd_t *__restrict, mtx_t *__restrict, const struct timespec *__restrict);
int cnd_wait(cnd_t *, mtx_t *);
void mtx_destroy(mtx_t *);
int mtx_init(mtx_t *, int);
int mtx_lock(mtx_t *);
int mtx_timedlock(mtx_t *__restrict, const struct timespec *__restrict);
int mtx_trylock(mtx_t *);
int mtx_unlock(mtx_t *);
int thrd_create(thrd_t *, thrd_start_t, void *);
thrd_t thrd_current(void);
int thrd_detach(thrd_t);
int thrd_equal(thrd_t, thrd_t);
_Noreturn void thrd_exit(int);
int thrd_join(thrd_t, int *);
int thrd_sleep(const struct timespec *, struct timespec *);
void thrd_yield(void);
int tss_create(tss_t *, tss_dtor_t);
void tss_delete(tss_t);
void *tss_get(tss_t);
int tss_set(tss_t, void *);


#ifdef __cplusplus
}
#endif
#endif
