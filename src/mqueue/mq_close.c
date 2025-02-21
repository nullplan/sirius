#include <mqueue.h>
#include "syscall.h"

int mq_close(mqd_t mq)
{
    int rv = __syscall(SYS_close, mq);
    if (rv == -EINTR) rv = 0;
    return __syscall_ret(rv);
}
