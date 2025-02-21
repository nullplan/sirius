#include <mqueue.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/socket.h>
#include "syscall.h"

struct args {
    int sk;
    union sigval sv;
    void (*fct)(union sigval);
    sem_t done;
};

static void *notify_thread(void *args)
{
    struct args *a = args;
    int sk = a->sk;
    union sigval sv = a->sv;
    void (*fct)(union sigval) = a->fct;
    sem_post(&a->done);
    unsigned char buf[32];
    for (;;)
    {
        int rv = recv(sk, buf, sizeof buf, 0);
        if (rv == sizeof buf) {
           if (buf[31] == 1)
            fct(sv);
           break;
        }
    }
    __syscall(SYS_close, sk);
    return 0;
}

int mq_notify(mqd_t mq, const struct sigevent *se)
{
    if (!se || se->sigev_notify != SIGEV_THREAD)
        return syscall(SYS_mq_notify, mq, se);

    int sk = socket(AF_NETLINK, SOCK_DGRAM, 0);
    if (sk == -1)
    {
        errno = EAGAIN;
        return -1;
    }

    unsigned char buf[32] = {0};
    struct sigevent lse;
    lse.sigev_notify = SIGEV_THREAD;
    lse.sigev_signo = sk;
    lse.sigev_value.sival_ptr = buf;
    if (__syscall(SYS_mq_notify, mq, &lse)) {
        __syscall(SYS_close, sk);
        errno = EAGAIN;
        return -1;
    }

    pthread_attr_t a;
    if (se->sigev_notify_attributes)
        a = *se->sigev_notify_attributes;
    else {
        pthread_attr_init(&a);
        pthread_attr_setdetachstate(&a, PTHREAD_CREATE_DETACHED);
    }

    struct args args;
    pthread_t t;
    args.sk = sk;
    args.sv = se->sigev_value;
    args.fct = se->sigev_notify_function;
    sem_init(&args.done, 0, 0);
    if (pthread_create(&t, &a, notify_thread, &args)) {
        __syscall(SYS_mq_notify, mq, 0);
        __syscall(SYS_close, sk);
        errno = EAGAIN;
        return -1;
    }
    sem_wait(&args.done);
    return 0;
}
