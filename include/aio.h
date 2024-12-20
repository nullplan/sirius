#ifndef __AIO_H
#define __AIO_H

#define __NEED_off_t
#define __NEED_size_t
#define __NEED_union_sigval
#define __NEED_struct_sigevent
#define __NEED_pthread_attr_t
#define __NEED_ssize_t
#define __NEED_time_t
#define __NEED_struct_timespec
#include <alltypes.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

struct aiocb {
    int aio_fildes;
    off_t aio_offset;
    volatile void *aio_buf;
    size_t aio_nbytes;
    int aio_reqprio;
    struct sigevent aio_sigevent;
    int aio_lio_opcode;
};

#define AIO_ALLDONE (-2)
#define AIO_CANCELED (-3)
#define AIO_NOTCANCELED (-4)

#define LIO_NOWAIT  0
#define LIO_WAIT    1
#define LIO_NOP 0
#define LIO_READ 1
#define LIO_WRITE 2

int aio_cancel(int, struct aiocb *);
int aio_error(int, struct aiocb *);
int aio_fsync(int, struct aiocb *);
int aio_read(struct aiocb *);
ssize_t aio_return(struct aiocb *);
int aio_suspend(const struct aiocb *const [], int, const struct timespec *);
int aio_write(struct aiocb *);
int lio_listio(int, struct aiocb *__restrict const [__restrict]

#ifdef __cplusplus
}
#endif
#endif
