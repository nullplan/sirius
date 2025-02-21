#include <mqueue.h>

ssize_t mq_receive(mqd_t mq, char *buf, size_t len, unsigned *prio)
{
    return mq_timedreceive(mq, buf, len, prio, 0);
}
