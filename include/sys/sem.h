#ifndef __SYS_SEM_H
#define __SYS_SEM_H

#define __NEED_pid_t
#define __NEED_size_t
#define __NEED_time_t
#include <alltypes.h>
#include <sys/ipc.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

#include <bits/sem.h>

#define SEM_UNDO        0x1000
#define GETPID          11
#define GETVAL          12
#define GETALL          13
#define GETNCNT         14
#define GETZCNT         15
#define SETVAL          16
#define SETALL          17

struct sembuf {
    unsigned short sem_num;
    short sem_op;
    short sem_flg;
};

int semctl(int, int, int, ...);
int semget(key_t, int, int);
int semop(int, struct sembuf *, size_t);

#ifdef __cplusplus
}
#endif
#endif
