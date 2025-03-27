#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <string.h>
#include "syscall.h"
#include "libc.h"

static int synccall_target;
static sem_t entry_sem, pre_func_sem, post_func_sem, exit_sem, done_sem;
static void (*synccall_cb)(void *);
static void *synccall_arg;

static void do_nothing(void *foo)
{
}

static void synccall_handler(int foo)
{
    if (synccall_target != __pthread_self()->tid)
        return;

    sem_post(&entry_sem);
    sem_wait(&pre_func_sem);
    synccall_cb(synccall_arg);
    sem_post(&post_func_sem);
    sem_wait(&exit_sem);
    sem_post(&done_sem);
}

hidden void __synccall(void (*cb)(void *), void *arg)
{
    /* We were never MT, so just run the callback and return. */
    if (!__threaded) {
        cb(arg);
        return;
    }

    sigset_t ss;
    __block_usr_sigs(&ss);
    __tl_lock();
    __block_all_sigs(0);
    pthread_t self = __pthread_self();

    sem_init(&entry_sem, 0, 0);
    sem_init(&pre_func_sem, 0, 0);
    sem_init(&post_func_sem, 0, 0);
    sem_init(&exit_sem, 0, 0);
    sem_init(&done_sem, 0, 0);

    struct sigaction sa;
    sa.sa_handler = synccall_handler;
    sa.sa_flags = SA_RESTART | SA_ONSTACK;
    memset(&sa.sa_mask, -1, sizeof sa.sa_mask);
    __libc_sigaction(SIGSYNCCALL, &sa, 0);

    synccall_cb = cb;
    synccall_arg = arg;

    int captured = 0;
    int rv = 0;
    for (pthread_t t = self->next; rv == 0 && t != self; t = t->next) {
        if (t->tid) {
            synccall_target = t->tid;
            do rv = __syscall(SYS_tkill, t->tid, SIGSYNCCALL);
            while (rv == -EAGAIN);
            if (rv == 0) {
                sem_wait(&entry_sem);
                captured++;
            }
        }
    }

    sa.sa_handler = SIG_IGN;
    __libc_sigaction(SIGSYNCCALL, &sa, 0);

    if (rv) synccall_cb = cb = do_nothing;

    for (int i = 0; i < captured; i++)
    {
        sem_post(&pre_func_sem);
        sem_wait(&post_func_sem);
    }

    cb(arg);

    for (int i = 0; i < captured; i++)
        sem_post(&exit_sem);

    for (int i = 0; i < captured; i++)
        sem_wait(&done_sem);

    sem_destroy(&entry_sem);
    sem_destroy(&pre_func_sem);
    sem_destroy(&post_func_sem);
    sem_destroy(&exit_sem);
    sem_destroy(&done_sem);

    __tl_unlock();
    __restore_sigs(&ss);
}
