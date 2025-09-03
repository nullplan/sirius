#include <aio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <semaphore.h>
#include "futex.h"
#include "cpu.h"

/* Simple thread-based AIO implementation
 *
 * Reason for threads is that, while io_uring could do most of the things
 * required, it is not available in all supported kernel versions.
 *
 * For now I am implementing this similar to how musl does it:
 *
 * There is a queue of requests for each file descriptor. The queues are all
 * listed in a decimal tree of degree 256 and order 4. Each request spawns a
 * thread to handle it. Due to this, the request list nodes can be allocated on
 * each thread stack.
 *
 * fsync and append requests have to wait for all prior queued writes to
 * complete before beginning.
 */

static int num_aio_fd;
hidden int __aio_notify;
struct aio_queue;
struct aio_task {
    int op;
    int err;
    ssize_t result;
    volatile int running;
    struct aiocb *cb;
    pthread_t worker;
    struct aio_task *next, *prev;
    struct aio_queue *q;
};

struct aio_queue {
    int init;
    int seekable;
    int append;
    int count; /* number of references == number of outstanding requests + number of threads currently inserting stuff */
    pthread_mutex_t lock;
    pthread_cond_t task_done;
    struct aio_task *head;
};

static pthread_rwlock_t map_lock = PTHREAD_RWLOCK_INITIALIZER;
static struct aio_queue *****map;

static struct aio_queue *get_queue(int fd, int alloc)
{
    if (fd < 0) {
        errno = EBADF;
        return 0;
    }

    unsigned char a = fd >> 24;
    unsigned char b = fd >> 16;
    unsigned char c = fd >> 8;
    unsigned char d = fd;
    struct aio_queue *rv = 0;

    pthread_rwlock_rdlock(&map_lock);
    if ((!map || !map[a] || !map[a][b] || !map[a][b][c] || !(rv = map[a][b][c][d])) && alloc) {
        pthread_rwlock_unlock(&map_lock);
        pthread_rwlock_wrlock(&map_lock);
        if (!map) map = __libc_calloc(128, sizeof *map);
        if (!map) goto out_unlock;
        if (!map[a]) map[a] = __libc_calloc(256, sizeof **map);
        if (!map[a]) goto out_unlock;
        if (!map[a][b]) map[a][b] = __libc_calloc(256, sizeof ***map);
        if (!map[a][b]) goto out_unlock;
        if (!map[a][b][c]) map[a][b][c] = __libc_calloc(256, sizeof ****map);
        if (!map[a][b][c]) goto out_unlock;
        rv = map[a][b][c][d];
        if (!rv) {
            rv = map[a][b][c][d] = __libc_malloc(sizeof *****map);
            if (!rv) goto out_unlock;
            rv->init = 0;
            pthread_mutex_init(&rv->lock, 0);
            pthread_cond_init(&rv->task_done, 0);
            rv->head = 0;
            rv->count = 0;
            a_inc(&num_aio_fd);
        }
    }

    if (rv)
        pthread_mutex_lock(&rv->lock);

out_unlock:
    pthread_rwlock_unlock(&map_lock);
    return rv;
}

static int has_write_operation(const struct aio_task *t)
{
    for (; t; t = t->next)
        if (t->op == LIO_WRITE)
            return 1;
    return 0;
}

static void unref_queue(struct aio_queue *q, int fd)
{
    if (q->count > 1) {
        q->count--;
        pthread_mutex_unlock(&q->lock);
        return;
    }

    /* if q->count == 1, this might be the last ref,
     * and I need to remove it. But I cannot remove
     * anything from the map since for that I need
     * to take the map write lock.
     * But I can only take that lock if I don't have
     * the queue lock, since otherwise this function might
     * deadlock with get_queue().
     *
     * The queue cannot be removed in that time since we are still holding
     * one reference.
     */
    pthread_mutex_unlock(&q->lock);
    pthread_rwlock_wrlock(&map_lock);
    pthread_mutex_lock(&q->lock);
    unsigned char a, b, c, d;
    a = fd >> 24;
    b = fd >> 16;
    c = fd >> 8;
    d = fd;

    if (q->count == 1) {
        /* nobody has added a reference while we were taking the map lock. */
        map[a][b][c][d] = 0;
        pthread_mutex_unlock(&q->lock);
        pthread_mutex_destroy(&q->lock);
        pthread_cond_destroy(&q->task_done);
        __libc_free(q);
        a_dec(&num_aio_fd);
    } else {
        /* somebody did add a ref, so we must remove ours. */
        q->count--;
        pthread_mutex_unlock(&q->lock);
    }
    pthread_rwlock_unlock(&map_lock);
}

static void cleanup(void *ctx)
{
    struct aio_task *t = ctx;
    struct aio_queue *q = t->q;
    if (a_swap(&t->running, 0) < 0)
        __futex_wake(&t->running, 1, INT_MAX);
    t->cb->__res = t->result;
    if (a_swap(&t->cb->__err, t->err) != EINPROGRESS)
        __futex_wake(&t->cb->__err, 1, INT_MAX);
    int val, nval;
    do {
        val = __aio_notify;
        nval = (val + 1u) & INT_MAX;
    } while (a_cas(&__aio_notify, val, nval) != val);
    if (val < 0)
        __futex_wake(&__aio_notify, 1, INT_MAX);

    pthread_mutex_lock(&q->lock);
    if (t->next) t->next->prev = t->prev;
    if (t->prev) t->prev->next = t->next;
    else q->head = t->next;
    pthread_cond_broadcast(&q->task_done);
    unref_queue(q, t->cb->aio_fildes);
    switch (t->cb->aio_sigevent.sigev_notify)
    {
        case SIGEV_SIGNAL:
            sigqueue(getpid(), t->cb->aio_sigevent.sigev_signo, t->cb->aio_sigevent.sigev_value);
            break;
        case SIGEV_THREAD:
            /* the notification function is supposed to run in a new thread
             * that didn't have a handle to cancel before.
             * Although cancellation is disabled if we were canceled, the function
             * can turn it back on and then immediately fall over.
             * So I turn the cancel marker off and reset the cancel state.
             */
            a_swap(&__pthread_self()->cancel, 0);
            __pthread_self()->cancelstate = 0;
            t->cb->aio_sigevent.sigev_notify_function(t->cb->aio_sigevent.sigev_value);
            break;
    }
}

struct args {
    struct aio_queue *q;
    struct aiocb *cb;
    int op;
    sem_t sem;
};

static void *worker_thread(void *ctx)
{
    struct args *a = ctx;
    struct aio_queue *q = a->q;
    struct aio_task task;
    struct aio_task *t = &task;
    struct aiocb *cb = a->cb;
    int fd = cb->aio_fildes;
    int seekable, append;
    int op = a->op;
    sem_post(&a->sem);
    pthread_mutex_lock(&q->lock);

    t->op = op;
    t->err = ECANCELED;
    t->result = -1;
    t->running = 1;
    t->cb = cb;
    t->worker = __pthread_self();
    t->next = q->head;
    if (t->next) t->next->prev = t;
    t->prev = 0;
    q->head = t;

    if (!q->init) {
        q->seekable = lseek(fd, 0, SEEK_CUR) >= 0;
        q->append = q->seekable && (fcntl(fd, F_GETFL) & O_APPEND);
        q->init = 1;
    }
    seekable = q->seekable;
    append = q->append;

    if (op == O_SYNC || op == O_DSYNC || (op == LIO_WRITE && q->append)) {
        while (has_write_operation(t->next))
            pthread_cond_wait(&q->task_done, &q->lock);
    }

    pthread_mutex_unlock(&q->lock);

    pthread_cleanup_push(cleanup, t);
    ssize_t rv = -1;
    switch (op) {
        case LIO_READ:
            rv = seekable?
                pread(fd, (void *)cb->aio_buf, cb->aio_nbytes, cb->aio_offset) :
                read(fd, (void *)cb->aio_buf, cb->aio_nbytes);
            break;
        case LIO_WRITE:
            rv = !append?
                pwrite(fd, (void *)cb->aio_buf, cb->aio_nbytes, cb->aio_offset) :
                write(fd, (void *)cb->aio_buf, cb->aio_nbytes);
            break;
        case O_DSYNC: rv = fdatasync(fd); break;
        case O_SYNC: rv = fsync(fd); break;
    }

    t->result = rv;
    t->err = rv >= 0? 0 : errno;
    pthread_cleanup_pop(1);
    return 0;
}

static int submit_masked(struct aiocb *cb, int op)
{
    struct aio_queue *q = get_queue(cb->aio_fildes, 1);
    if (!q) {
        if (errno != EBADF)
            errno = EAGAIN;
        return -1;
    }

    q->count++;
    pthread_attr_t a;
    if (cb->aio_sigevent.sigev_notify == SIGEV_THREAD) {
        if (cb->aio_sigevent.sigev_notify_attributes)
            a = *cb->aio_sigevent.sigev_notify_attributes;
        else
            pthread_attr_init(&a);
    } else {
        pthread_attr_init(&a);
        pthread_attr_setstacksize(&a, PTHREAD_STACK_MIN);
    }

    struct args args;
    args.q = q;
    args.op = op;
    args.cb = cb;
    cb->__err = EINPROGRESS;
    cb->__res = -1;
    pthread_t td;
    sem_init(&args.sem, 0, 0);
    pthread_attr_setdetachstate(&a, PTHREAD_CREATE_DETACHED);
    int rv = pthread_create(&td, &a, worker_thread, &args);
    pthread_attr_destroy(&a);
    if (!rv)
        q->count++; /* for the successfully created thread. */
    unref_queue(q, cb->aio_fildes);

    if (rv)
    {
        errno = EAGAIN;
        return -1;
    }

    sem_wait(&args.sem);
    sem_destroy(&args.sem);
    return 0;
}

static int submit(struct aiocb *cb, int op)
{
    sigset_t maskset, oldset;
    sigfillset(&maskset);
    pthread_sigmask(SIG_BLOCK, &maskset, &oldset);
    int rv = submit_masked(cb, op);
    pthread_sigmask(SIG_SETMASK, &oldset, 0);
    return rv;
}

int aio_read(struct aiocb *cb)
{
    return submit(cb, LIO_READ);
}

int aio_write(struct aiocb *cb)
{
    return submit(cb, LIO_WRITE);
}

int aio_fsync(int flag, struct aiocb *cb)
{
    if (flag != O_SYNC && flag != O_DSYNC)
    {
        errno = EINVAL;
        return -1;
    }
    return submit(cb, flag);
}

int aio_error(struct aiocb *cb)
{
    return cb->__err & 0x7fffffff;
}

ssize_t aio_return(struct aiocb *cb)
{
    return cb->__res;
}

int aio_cancel(int fd, struct aiocb *cb)
{
    if (cb && fd != cb->aio_fildes)
    {
        errno = EINVAL;
        return -1;
    }

    sigset_t maskset, oldset;
    sigfillset(&maskset);
    pthread_sigmask(SIG_BLOCK, &maskset, &oldset);
    /* In this implementation everything can be canceled, but it is unknown
     * if the kernel honors the request. If it does, the task is canceled.
     * If not, it completes. Either way, I don't get a "could not cancel"
     * notification.
     *
     * So in total, I will return AIO_CANCELED from this call if 
     * *any* call could be canceled, else AIO_ALLDONE.
     */
    int rv = AIO_ALLDONE;
    struct aio_queue *q = get_queue(fd, 0);
    if (!q) {
        if (fd < 0)
            rv = -1;
        goto out_unmask;
    }

    for (struct aio_task *t = q->head; t; t = t->next)
        if (!cb || cb == t->cb) {
            if (a_cas(&t->running, 1, -1)) {
                pthread_cancel(t->worker);
                while (t->running)
                    __futex_wait(&t->running, 1, -1);
                if (t->err == ECANCELED) rv = AIO_CANCELED;
            }
        }
    pthread_mutex_unlock(&q->lock);
out_unmask:
    pthread_sigmask(SIG_SETMASK, &oldset, 0);
    return rv;
}

void __aio_close(int fd)
{
    a_barrier();
    if (!num_aio_fd) return;
    aio_cancel(fd, 0);
}
