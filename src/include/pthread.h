#ifndef PTHREAD_H
#define PTHREAD_H

#include "../../include/pthread.h"
#include <stdint.h>
#include <locale.h>
#include "pthread_arch.h"
#include "tls.h"

struct lock { volatile int l; };
struct __pthread
{
    struct __pthread *self;     /* i386, x32, x86_64 ABI: first word is pointer to self */
    struct __pthread *next, *prev;
    size_t sysinfo;             /* internal i386 ABI: fourth word is sysinfo pointer */
    #ifdef TLS_VARIANT_2
    size_t *dtv;                /* internal i386 ABI: fifth word is DTV pointer. */
    #ifdef CANARY_PAD
    size_t canary_pad;          /* x32 ABI: canary is the *seventh* word, for some reason. */
    #endif
    size_t canary;              /* i386, x86_64 ABI: canary is the *sixth* word. */
    size_t hwcap;               /* internal i386 ABI: seventh word is hwcap. */
    #endif

    /* non-ABI free-for-all */
    int errno_val;
    int tid;
    struct lock killlock;
    int detachstate;
    int cancelstate;
    int cancel;
    locale_t locale;
    struct __file *locked_files;
    void *map;
    size_t map_size;
    void **tsd;
    void *result;
    struct __ptcb *cb;
    struct {
        struct __mtx *head;
        long off;
        struct __mtx *pending;
    } robust;
    char *dlerr_msg;
    char *dlerr_alloc;
    size_t dlerr_alloc_size;

    #ifndef TLS_VARIANT_2
    size_t hwcap;               /* internal PowerPC, PowerPC64 ABI: hwcap is third word before the end. */
    size_t canary;              /* PowerPC, PowerPC64 ABI: canary is the penultimate word. */
    size_t *dtv;                /* Aarch64 internal ABI: dtv is last word of thread descriptor. */
    #endif
};

enum {DT_JOINABLE, DT_DETACHED, DT_EXITING};

#ifndef TP_OFFSET
#define TP_OFFSET 0
#endif
#ifndef GAP_ABOVE_TP
#define GAP_ABOVE_TP 0
#endif
#ifndef DTV_OFFSET
#define DTV_OFFSET 0
#endif
#ifdef TLS_VARIANT_2
static inline uintptr_t __tp_adjust(struct __pthread *p) { return (uintptr_t)p; }
static inline struct __pthread *__pthread_self(void) { return (struct __pthread *)__get_tp(); }
#else
static inline uintptr_t __tp_adjust(struct __pthread *p) { return (uintptr_t)p + sizeof (struct __pthread) + TP_OFFSET; }
static inline struct __pthread *__pthread_self(void) { return (struct __pthread *)(__get_tp() - TP_OFFSET - sizeof (struct __pthread)); }
#endif

/* programming model: call __set_thread_area() with adjusted thread pointer once in the main thread.
 * Then afterwards the __clone() functions with CLONE_SETTLS and tls set to the adjusted new thread pointer
 * will spawn a new thread with the thread pointer set correctly.
 *
 * Funnily enough, this connection only exists on i386.
 */
extern hidden int __set_thread_area(uintptr_t);

/* programming model: Setting a stack value requires setting a function pointer.
 * So either both of the first two args are zero or nonzero.
 * Calling without function pointer requires absence of CLONE_VM,
 * else carnage ensues.
 */
extern hidden int __clone(int (*)(void *), void *, int, void *, int *, uintptr_t, int *);

/* tunables */
#define MIN_DEFAULT_STACK_SIZE      (8 << 10)   /* minimum we allow the default stack size to be set to. */
#define DEFAULT_STACK_SIZE          (128 << 10) /* the default default stack size if nothing else is given. */
#define MAX_DEFAULT_STACK_SIZE      (8 << 20)   /* maximum we allow the default stack size to be set to. */

#define DEFAULT_GUARD_SIZE          (4 << 10)

#define FUTEX_TID_MASK              0x3fffffff
#define FUTEX_NR_TID_MASK           0x7fffffff
#define FUTEX_OWNER_DIED            0x40000000
#define FUTEX_WAITERS               0x80000000U

extern hidden void __tl_lock(void);
extern hidden void __tl_unlock(void);
extern hidden void __tl_sync(void);

extern hidden int __futex_wait(volatile int *, int, int);
extern hidden int __futex_wake(volatile int *, int, int);

extern hidden void __lock(struct lock *);
extern hidden void __unlock(struct lock *);

extern hidden const size_t __pthread_tsd_size;
extern hidden void __pthread_tsd_destroy(void);

extern hidden void __testcancel(void);

extern hidden void __synccall(void (*)(void *), void *);

/* arg == -1 for preparations, arg == 0 for parent handlers, arg == 1 for child handlers. */
extern hidden void __atfork_run(int);

extern hidden int __pthread_setcancelstate(int, int *);
extern hidden int __pthread_cond_clockwait(pthread_cond_t *restrict, pthread_mutex_t *restrict, clockid_t, const struct timespec *restrict);
extern hidden int __pthread_mutex_init(pthread_mutex_t *restrict, const pthread_mutexattr_t *restrict);
extern hidden int __pthread_mutex_lock(pthread_mutex_t *);
extern hidden int __pthread_mutex_unlock(pthread_mutex_t *);
extern hidden int __pthread_mutex_clocklock(pthread_mutex_t *restrict, clockid_t, const struct timespec *restrict);
extern hidden int __pthread_mutex_trylock(pthread_mutex_t *);
extern hidden int __pthread_rwlock_clockwrlock(pthread_rwlock_t *restrict, clockid_t, const struct timespec *restrict);
extern hidden int __pthread_rwlock_timedwrlock(pthread_rwlock_t *restrict, const struct timespec *restrict);
extern hidden int __pthread_rwlock_wrlock(pthread_rwlock_t *);
extern hidden int __pthread_rwlock_unlock(pthread_rwlock_t *);
extern hidden int __pthread_rwlock_clockrdlock(pthread_rwlock_t *restrict, clockid_t, const struct timespec *restrict);
extern hidden int __pthread_rwlock_timedrdlock(pthread_rwlock_t *restrict, const struct timespec *restrict);
extern hidden int __pthread_rwlock_rdlock(pthread_rwlock_t *);
extern hidden int __pthread_key_create(pthread_key_t *, void (*)(void*));
extern hidden int __pthread_key_delete(pthread_key_t);
extern hidden void *__pthread_getspecific(pthread_key_t);
extern hidden int __pthread_setspecific(pthread_key_t, const void *);
extern hidden int __pthread_detach(pthread_t);
extern hidden void __pthread_testcancel(void);
extern hidden int __pthread_join(pthread_t, void **);
extern hidden _Noreturn void __pthread_exit(void *);
extern hidden int __pthread_create(pthread_t *restrict, const pthread_attr_t *restrict, void *(*)(void*), void *restrict);

#define __PTHREAD_ATTR_C11 0x80000000u
#endif
