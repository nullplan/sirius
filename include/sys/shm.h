#ifndef __SYS_SHM_H
#define __SYS_SHM_H

#define __NEED_intptr_t
#define __NEED_pid_t
#define __NEED_size_t
#define __NEED_time_t
#include <alltypes.h>
#include <features.h>
#include <sys/ipc.h>
#ifdef __cplusplus
extern "C" {
#endif

#define SHM_RDONLY  0x1000
#define SHM_RND     0x2000
#define SHM_FAILED  ((void *)-1)
#define SHMLBA      4096

#include <bits/shm.h>

void *shmat(int, const void *, int);
int shmctl(int, int, struct shmid_ds *);
int shmdt(const void *);
int shmget(key_t, size_t, int);

#ifdef __cplusplus
}
#endif
#endif
