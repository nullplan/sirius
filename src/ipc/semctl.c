#include <sys/sem.h>
#include <stdarg.h>
#include "syscall.h"

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int semctl(int id, int num, int cmd, ...)
{
    unsigned long arg = 0;
    va_list ap;
    va_start(ap, cmd);
    switch (cmd) {
        case SETVAL:
            arg = va_arg(ap, union semun).val;
            break;
        case GETALL:
        case SETALL:
            arg = (unsigned long)(va_arg(ap, union semun).array);
            break;
        case IPC_STAT:
        case IPC_SET:
            arg = (unsigned long)(va_arg(ap, union semun).buf);
            break;
    }
    va_end(ap);
    #ifdef SYS_ipc
    return syscall(SYS_ipc, 3, id, num, cmd | IPC_64, &arg);
    #else
    return syscall(SYS_semctl, id, num, cmd, arg);
    #endif
}
