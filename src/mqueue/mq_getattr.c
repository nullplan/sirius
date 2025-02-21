#include <mqueue.h>

int mq_getattr(mqd_t mq, struct mq_attr *ma)
{
    return mq_setattr(mq, 0, ma);
}
