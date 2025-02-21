#include <mqueue.h>

int mq_send(mqd_t mq, const char *buf, size_t len, unsigned prio)
{
    return mq_timedsend(mq, buf, len, prio, 0);
}
