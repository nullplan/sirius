#define _GNU_SOURCE
#include <pthread.h>
#include <signal.h>
#include <sched.h>
#include <sys/mman.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdalign.h>
#include "libc.h"
#include "syscall.h"
#include "cpu.h"
#include "futex.h"

static const size_t dummy = 0;
weak_alias(__pthread_tsd_size, dummy);

static void dummy0(void) {}
weak_alias(__stdio_init_locks, dummy0);

static size_t dummy1(void) { return 0; }
weak_alias(__next_canary, dummy1);

struct startup_args {
    void *(*func)(void *);
    void *arg;
    sigset_t *old_ss;
    int control;
};

/* control state machine:
 * 0 - wait for it to be non-zero
 * 1 - terminate self
 * 2 - read args
 * 3 - done reading
 */
static int startup(void *a)
{
    struct startup_args *args = a;
    void *(*f)(void *);

    while (!args->control)
        __futex_wait(&args->control, 1, 0);
    if (args->control == 1) {
        __syscall(SYS_set_tid_address, &args->control);
        return 0;
    }

    f = args->func;
    a = args->arg;
    __restore_sigs(args->old_ss);
    a_swap(&args->control, 3);
    __futex_wake(&args->control, 1, 1);
    pthread_exit(f(a));
}

int pthread_create(pthread_t *restrict td, const pthread_attr_t *restrict a, void *(*func)(void*), void *restrict arg)
{
    const int flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_PARENT | CLONE_THREAD | CLONE_SYSVSEM | CLONE_SETTLS | CLONE_PARENT_SETTID | CLONE_CHILD_CLEARTID;
    struct tls_data tls_data = __get_tls_data();
    void *sp = 0;
    size_t guardsize = 0;
    size_t alloc = 0;
    void *thread_mem;
    size_t *tsd;
    pthread_t new_td = 0;

    if (a && a->__addr) {
        if (tls_data.size + tls_data.align + __pthread_tsd_size <= a->__ss / 8) {
            thread_mem = (void *)(((uintptr_t)a->__addr + a->__ss - tls_data.size) & -tls_data.align);
            tsd = (void *)(((uintptr_t)thread_mem - __pthread_tsd_size) & -alignof(size_t));
            sp = (void *)((uintptr_t)tsd & -16ul);
            memset(tsd, 0, (uintptr_t)thread_mem + tls_data.size - (uintptr_t)tsd);
            new_td = __copy_tls(thread_mem);
        } else {
            sp = (void *)(((uintptr_t)a->__addr + a->__ss) & -16);
            alloc = tls_data.size + __pthread_tsd_size;
            guardsize = 0;
        }
    } else {
        alloc = (a? a->__ss : __default_stacksize) + tls_data.size + __pthread_tsd_size;
        guardsize = a? a->__gs : DEFAULT_GUARD_SIZE;
    }

    if (alloc) {
        guardsize = (guardsize + PAGE_SIZE - 1) & -PAGE_SIZE;
        size_t map_size = guardsize + alloc;
        void *map = mmap(0, map_size, guardsize? PROT_NONE : PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (map == MAP_FAILED) return EAGAIN;
        if (guardsize && mprotect((char *)map + guardsize, alloc, PROT_READ | PROT_WRITE)) {
            munmap(map, guardsize + alloc);
            return EAGAIN;
        }
        thread_mem = (void *)(((uintptr_t)map + guardsize + alloc - tls_data.size) & -tls_data.align);
        tsd = (void *)(((uintptr_t)thread_mem - __pthread_tsd_size) & -alignof(size_t));
        if (!sp) sp = (void *)((uintptr_t)tsd & -16ul);
        new_td = __copy_tls(thread_mem);
        new_td->map = map;
        new_td->map_size = map_size;
    }
    new_td->tsd = (void **)tsd;
    new_td->self = new_td;

    pthread_t self = __pthread_self();
    new_td->sysinfo = self->sysinfo;
    new_td->hwcap = self->hwcap;
    new_td->locale = &__global_locale;
    if (a && (a->__flags & PTHREAD_CREATE_DETACHED))
        new_td->detachstate = DT_DETACHED;

    if (!__threaded) {
        __stdio_init_locks();
        __threaded = 1;
    }

    sigset_t ss;
    __block_usr_sigs(&ss);
    __tl_lock();

    new_td->canary = __next_canary();

    struct startup_args startup_args = {func, arg, &ss, 0};
    if (!a || !(a->__flags & PTHREAD_EXPLICIT_SCHED))
        startup_args.control = 2;
    int tid = __clone(startup, sp, flags, &startup_args, &new_td->tid, __tp_adjust(new_td), &__thread_list_lock);
    int rv = EAGAIN;
    if (tid < 0) goto out_unlock;
    rv = 0;
    if (a && (a->__flags & PTHREAD_EXPLICIT_SCHED)) {
        rv = -__syscall(SYS_sched_setscheduler, tid, a->__pol, &a->__prio);
        if (rv) {
            a_swap(&startup_args.control, 1);
            __futex_wake(&startup_args.control, 1, 1);
            while (startup_args.control)
                __futex_wait(&startup_args.control, 0, 1);
            goto out_unlock;
        }
        a_swap(&startup_args.control, 2);
        __futex_wake(&startup_args.control, 1, 1);
    }

    new_td->next = self->next;
    self->next = new_td;
    new_td->prev = self;
    new_td->next->prev = new_td;
    *td = new_td;
    while (startup_args.control != 3)
        __futex_wait(&startup_args.control, 1, 2);

out_unlock:
    __tl_unlock();
    __restore_sigs(&ss);
    if (rv && new_td->map) munmap(new_td->map, new_td->map_size);
    return rv;
}
