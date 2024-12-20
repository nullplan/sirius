#ifndef __MQUEUE_H
#define __MQUEUE_H

#define __NEED_size_t
#define __NEED_ssize_t
#define __NEED_time_t
#define __NEED_struct_timespec
#include <alltypes.h>
#include <features.h>
#include <fcntl.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef int mqd_t;
struct sigevent;

#if defined __x86_64__ && defined __ILP32__
/* ugly but apparently necessary. Thank you Linus. */
struct mq_attr {
    long mq_flags;
    long __r0;
    long mq_maxmsg;
    long __r1;
    long mq_msgsize;
    long __r2;
    long mq_curmsgs;
    long __r3;
    long long __reserved[4];
};
#else
struct mq_attr {
    long mq_flags;
    long mq_maxmsg;
    long mq_msgsize;
    long mq_curmsgs;
    long __reserved[4];
};
#endif

int mq_close(mqd_t);
int mq_getattr(mqd_t, struct mq_attr *);
int mq_notify(mqd_t, const struct sigevent *);
mqd_t mq_open(const char *, int, ...);
ssize_t mq_receive(mqd_t, char *, size_t, unsigned *);
int mq_send(mqd_t, const char *, size_t, unsigned);
int mq_setattr(mqd_t, const struct mq_attr *__restrict, struct mq_attr *__restrict);
ssize_t mq_timedreceive(mqd_t, char *__restrict, size_t, unsigned *__restrict, const struct timespec *__restrict);
int mq_timedsend(mqd_t, const char *, size_t, unsigned, const struct timespec *);
int mq_unlink(const char *);
#ifdef __cplusplus
}
#endif
#endif
