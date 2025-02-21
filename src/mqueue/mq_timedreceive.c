#include <mqueue.h>
#include "syscall.h"

ssize_t mq_timedreceive(mqd_t mq, char *restrict buf, size_t len, unsigned *restrict prio, const struct timespec *restrict ts)
{
    return syscall(SYS_mq_timedreceive, mq, buf, len, prio, ts);
}
