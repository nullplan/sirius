#include <sys/mman.h>
#include "syscall.h"

int mlockall(int flg)
{
    return syscall(SYS_mlockall, flg);
}
