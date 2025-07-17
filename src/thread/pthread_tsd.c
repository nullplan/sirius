#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <assert.h>
#include "libc.h"

static void *main_tsd[PTHREAD_KEYS_MAX];
hidden const size_t __pthread_tsd_size = sizeof main_tsd;

hidden void __init_tsd(void)
{
    __pthread_self()->tsd = main_tsd;
}

static pthread_rwlock_t dtors_lock = PTHREAD_RWLOCK_INITIALIZER;
static void (*dtors[PTHREAD_KEYS_MAX])(void *);
static void sentinel(void *) {}

hidden void __pthread_tsd_destroy(void)
{
    pthread_t self = __pthread_self();
    for (int i = 0; i < PTHREAD_DESTRUCTOR_ITERATIONS; i++)
    {
        int data_found = 0;
        if (__pthread_rwlock_rdlock(&dtors_lock)) return;
        for (size_t j = 0; j < PTHREAD_KEYS_MAX; j++)
        {
            if (self->tsd[j] && dtors[j] != sentinel) {
                data_found = 1;
                void *data = self->tsd[j];
                void (*dtor)(void *) = dtors[j];
                self->tsd[j] = 0;
                __pthread_rwlock_unlock(&dtors_lock);
                dtor(data);
                __pthread_rwlock_rdlock(&dtors_lock);
            }
        }
        __pthread_rwlock_unlock(&dtors_lock);
        if (!data_found) break;
    }
}

hidden int __pthread_key_create(pthread_key_t *k, void (*dtor)(void*))
{
    int rv = EAGAIN;
    if (!dtor) dtor = sentinel;
    __pthread_rwlock_wrlock(&dtors_lock);
    for (size_t i = 0; i < PTHREAD_KEYS_MAX; i++)
        if (!dtors[i]) {
            k->__v = i;
            dtors[i] = dtor;
            rv = 0;
            break;
        }
    __pthread_rwlock_unlock(&dtors_lock);
    return rv;
}
weak_alias(pthread_key_create, __pthread_key_create);

hidden int __pthread_key_delete(pthread_key_t k)
{
    sigset_t ss;
    __pthread_rwlock_wrlock(&dtors_lock);
    assert(dtors[k.__v]);
    dtors[k.__v] = 0;
    __block_usr_sigs(&ss);
    __tl_lock();
    pthread_t self = __pthread_self();
    pthread_t td = self;
    do td->tsd[k.__v] = 0;
    while ((td = td->next) != self);
    __tl_unlock();
    __restore_sigs(&ss);
    __pthread_rwlock_unlock(&dtors_lock);
    return 0;
}
weak_alias(pthread_key_delete, __pthread_key_delete);

hidden void *__pthread_getspecific(pthread_key_t k)
{
    return __pthread_self()->tsd[k.__v];
}
weak_alias(pthread_getspecific, __pthread_getspecific);

hidden int __pthread_setspecific(pthread_key_t k, const void *d)
{
    __pthread_self()->tsd[k.__v] = (void *)d;
    return 0;
}
weak_alias(pthread_setspecific, __pthread_setspecific);
