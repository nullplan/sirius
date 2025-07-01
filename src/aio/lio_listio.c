#include <aio.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <unistd.h>

struct waitarg {
    struct sigevent evt;
    size_t n;
    struct aiocb *cbs[];
};

static void *waiter(void *ctx)
{
    struct waitarg *args = ctx;
    for (size_t i = 0; i < args->n; i++)
        while (aio_suspend((void *)(args->cbs + i), 1, 0) < 0 && errno == EINTR);
    if (args->evt.sigev_notify == SIGEV_THREAD) {
        void (*f)(union sigval) = args->evt.sigev_notify_function;
        union sigval value = args->evt.sigev_value;
        free(args);
        f(value);
    } else {
        sigqueue(getpid(), args->evt.sigev_signo, args->evt.sigev_value);
        free(args);
    }
    return 0;

}

int lio_listio(int mode, struct aiocb *restrict const cbs[restrict], int n, struct sigevent *restrict evt)
{
    size_t nop = 0;
    for (int i = 0; i < n; i++)
        if (cbs[i] && (cbs[i]->aio_lio_opcode == LIO_READ || cbs[i]->aio_lio_opcode == LIO_WRITE)) {
            int rv = 0;
            nop++;
            if (cbs[i]->aio_lio_opcode == LIO_READ)
                rv = aio_read(cbs[i]);
            else
                rv = aio_write(cbs[i]);
            if (rv) {
                errno = EAGAIN;
                return -1;
            }
        }
    if (mode == LIO_WAIT) {
        for (int i = 0; i < n; i++)
            if (cbs[i] && (cbs[i]->aio_lio_opcode == LIO_READ || cbs[i]->aio_lio_opcode == LIO_WRITE))
            {
                int rv = aio_suspend((void *)(cbs + i), 1, 0);
                if (rv) return -1;
                if (aio_error(cbs[i])) {
                    errno = EIO;
                    return -1;
                }
            }
        return 0;
    }
    if (!nop || !evt || (evt->sigev_notify != SIGEV_SIGNAL && evt->sigev_notify != SIGEV_THREAD)) return 0;

    struct waitarg *w = malloc(sizeof (struct waitarg) + nop * sizeof (struct aiocb *));
    if (!w) {
        errno = EAGAIN;
        return -1;
    }
    w->evt = *evt;
    w->n = nop;
    nop = 0;
    for (int i = 0; i < n; i++)
        if (cbs[i] && (cbs[i]->aio_lio_opcode == LIO_READ || cbs[i]->aio_lio_opcode == LIO_WRITE))
            w->cbs[nop++] = cbs[i];
    pthread_attr_t a;
    pthread_t td;
    if (evt && evt->sigev_notify == SIGEV_THREAD && evt->sigev_notify_attributes)
        a = *evt->sigev_notify_attributes;
    else {
        pthread_attr_init(&a);
        pthread_attr_setstacksize(&a, PTHREAD_STACK_MIN);
    }
    pthread_attr_setdetachstate(&a, PTHREAD_CREATE_DETACHED);
    int rv = pthread_create(&td, &a, waiter, w);
    pthread_attr_destroy(&a);
    if (rv) {
        errno = EAGAIN;
        return -1;
    }
    return 0;
}
