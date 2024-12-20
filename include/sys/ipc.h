#ifndef __SYS_IPC_H
#define __SYS_IPC_H

#define __NEED_uid_t
#define __NEED_gid_t
#define __NEED_mode_t
#define __NEED_key_t
#include <alltypes.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

#ifdef _GNU_SOURCE
#define __NSG(x)    x
#else
#define __NSG(x)    __##x
#endif

#include <bits/ipc.h>

#define IPC_PRIVATE ((key_t)0)

#define IPC_64      0x100
#define IPC_CREAT   0x200
#define IPC_EXCL    0x400
#define IPC_NOWAIT  0x800

#define IPC_RMID    0
#define IPC_SET     1
#define IPC_STAT    2

#ifdef _GNU_SOURCE
#define IPC_INFO    3
#endif

key_t ftok(const char *, int);

#ifdef __cplusplus
}
#endif
#endif
