#ifndef PTHREAD_H
#define PTHREAD_H

#include "../../include/pthread.h"
#include <stdint.h>
#include <locale.h>
#include "pthread_arch.h"

struct lock { volatile int l; };
struct __pthread
{
    struct __pthread *self;     /* i386, x32, x86_64 ABI: first word is pointer to self */
    struct __pthread *next, *prev;
    size_t sysinfo;             /* internal i386 ABI: fourth word is sysinfo pointer */
    #ifdef TLS_VARIANT_1
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
    size_t *tsd;
    void *result;
    struct __ptcb *cb;

    #ifndef TLS_VARIANT_1
    size_t hwcap;               /* internal PowerPC, PowerPC64 ABI: hwcap is third word before the end. */
    size_t canary;              /* PowerPC, PowerPC64 ABI: canary is the penultimate word. */
    size_t *dtv;
    #endif
};

enum {DT_JOINABLE, DT_DETACHED, DT_EXITING};

struct tls_data {
    size_t size;
    size_t align;
};
hidden struct tls_data __get_tls_data(void);
/* programming model: call __copy_tls() with zeroed out memory, with size and alignment given in the TLS data,
 * and it returns the TD placed correctly and TLS and DTV (and __pthread::dtv) filled out correctly
 * However, TSD is not part of this and might need to be allocated separately.
 */
hidden struct __pthread *__copy_tls(void *);

#ifndef TP_OFFSET
#define TP_OFFSET 0
#endif
#ifndef GAP_ABOVE_TP
#define GAP_ABOVE_TP 0
#endif
#ifndef DTV_OFFSET
#define DTV_OFFSET 0
#endif
#ifdef TLS_VARIANT_1
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
#define FUTEX_OWNER_DIED            0x40000000
#define FUTEX_WAITERS               0x80000000U

extern hidden void __tl_lock();
extern hidden void __tl_unlock();
extern hidden void __tl_sync();

extern hidden int __futex_wait(volatile int *, int, int);
extern hidden int __futex_wake(volatile int *, int, int);

extern hidden void __lock(struct lock *);
extern hidden void __unlock(struct lock *);

extern hidden const size_t __pthread_tsd_size;
extern hidden void __pthread_tsd_destroy(void);

extern hidden void __testcancel(void);
#endif
