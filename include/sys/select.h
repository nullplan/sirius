#ifndef __SYS_SELECT_H
#define __SYS_SELECT_H

#define __NEED_time_t
#define __NEED_suseconds_t
#define __NEED_struct_timeval
#define __NEED_struct_timespec
#include <alltypes.h>
#include <features.h>
#include <signal.h>
#ifdef __cplusplus
extern "C" {
#endif

#define FD_SETSIZE  1024
typedef struct {
    unsigned long __fds[FD_SETSIZE/8/sizeof(long)];
} fd_set;

int pselect(int, fd_set *__restrict, fd_set *__restrict, fd_set *__restrict, const struct timespec *__restrict, const sigset_t *__restrict);
int select(int, fd_set *__restrict, fd_set *__restrict, fd_set *__restrict, struct timeval *__restrict);

#define FD_ZERO(fds) do { \
    for (int __i = 0; __i < FD_SETSIZE/8/sizeof(long); __i++) \
        (fds)->__fds[__i] = 0; \
} while (0)

#define FD_CLR(fd, fds)     ((fds)->__fds[(fd)/(8*sizeof(long))] &=~ (1ul << ((fd) % (8*sizeof(long)))))
#define FD_SET(fd, fds)     ((fds)->__fds[(fd)/(8*sizeof(long))] |=  (1ul << ((fd) % (8*sizeof(long)))))
#define FD_ISSET(fd, fds)   ((fds)->__fds[(fd)/(8*sizeof(long))] &   (1ul << ((fd) % (8*sizeof(long)))))

#ifdef __cplusplus
}
#endif
#endif
