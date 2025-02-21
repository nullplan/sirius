#include <mqueue.h>
#include "syscall.h"

int mq_setattr(mqd_t mq, const struct mq_attr *restrict na, struct mq_attr *restrict oa)
{
    return syscall(SYS_mq_getsetattr, mq, na, oa);
}
