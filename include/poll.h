#ifndef __POLL_H
#define __POLL_H

#define __NEED_time_t
#define __NEED_struct_timespec
#include <alltypes.h>
#include <signal.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

struct pollfd {
    int fd;
    short events;
    short revents;
};

#define POLLIN      0x0001
#define POLLPRI     0x0002
#define POLLOUT     0x0004
#define POLLERR     0x0008
#define POLLHUP     0x0010
#define POLLNVAL    0x0020
#define POLLRDNORM  0x0040
#define POLLRDBAND  0x0080
#define POLLWRNORM  0x0100
#define POLLWRBAND  0x0200

typedef unsigned int nfds_t;

int poll(struct pollfd [], nfds_t, int);
int ppoll(struct pollfd [], nfds_t, const struct timespec *__restrict, const sigset_t *__restrict);

#ifdef __cplusplus
}
#endif
#endif
