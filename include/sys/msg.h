#ifndef __SYS_MSG_H
#define __SYS_MSG_H

#define __NEED_pid_t
#define __NEED_size_t
#define __NEED_ssize_t
#define __NEED_time_t
#include <alltypes.h>
#include <sys/ipc.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

#include <bits/msg.h>

int msgctl(int, int, struct msqid_ds *);
int msgget(key_t, int);
ssize_t msgrcv(int, void *, size_t, long, int);
int msgsnd(int, const void *, size_t, int);

#ifdef __cplusplus
}
#endif
#endif
