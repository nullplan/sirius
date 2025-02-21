#include <mqueue.h>
#include "syscall.h"

int mq_timedsend(mqd_t mq, const char *buf, size_t len, unsigned prio, const struct timespec *ts)
{
    return syscall(SYS_mq_timedsend, mq, buf, len, prio, ts);
}
