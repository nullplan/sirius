#include <mqueue.h>
#include "syscall.h"

int mq_unlink(const char *name)
{
    return syscall(SYS_mq_unlink, name);
}
