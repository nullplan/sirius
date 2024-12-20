#ifndef __SPAWN_H
#define __SPAWN_H

#define __NEED_mode_t
#define __NEED_pid_t
#include <alltypes.h>
#include <features.h>
#include <signal.h>

#ifdef __cplusplus
extern "C" {
#endif

struct sched_param;
struct __fa;

typedef struct {
    struct __fa *__f;
    struct __fa **__l;
} posix_spawn_file_actions_t;

typedef struct {
    unsigned __flg;
    pid_t __pgrp;
    unsigned long __msk[4];
    unsigned long __dfl[4];
} posix_spawnattr_t;

#define POSIX_SPAWN_RESETIDS            1
#define POSIX_SPAWN_SETPGROUP           2
#define POSIX_SPAWN_SETSID              4
#define POSIX_SPAWN_SETSIGDEF           8
#define POSIX_SPAWN_SETSIGMASK          16

int posix_spawn_file_actions_init(posix_spawn_file_actions_t *);
int posix_spawn_file_actions_addchdir(posix_spawn_file_actions_t *__restrict, const char *__restrict);
int posix_spawn_file_actions_addclose(posix_spawn_file_actions_t *, int);
int posix_spawn_file_actions_adddup2(posix_spawn_file_actions_t *, int, int);
int posix_spawn_file_actions_addfchdir(posix_spawn_file_actions_t *, int);
int posix_spawn_file_actions_addopen(posix_spawn_file_actions_t *__restrict, int, const char *__restrict, int, mode_t);
int posix_spawn_file_actions_destroy(posix_spawn_file_actions_t *);

int posix_spawnattr_init(posix_spawnattr_t *);
int posix_spawnattr_getflags(const posix_spawnattr_t *__restrict, short *__restrict);
int posix_spawnattr_getpgroup(const posix_spawnattr_t *__restrict, pid_t *__restrict);
int posix_spawnattr_getsigdefault(const posix_spawnattr_t *__restrict, sigset_t *__restrict);
int posix_spawnattr_getsigmask(const posix_spawnattr_t *__restrict, sigset_t *__restrict);
int posix_spawnattr_setflags(posix_spawnattr_t *, short);
int posix_spawnattr_setpgroup(posix_spawnattr_t *, pid_t);
int posix_spawnattr_setsigdefault(posix_spawnattr_t *__restrict, const sigset_t *__restrict);
int posix_spawnattr_setsigmask(posix_spawnattr_t *__restrict, const sigset_t *__restrict);
int posix_spawnattr_destroy(posix_spawnattr_t *);

int posix_spawn(pid_t *__restrict, const char *__restrict, const posix_spawn_file_actions_t *__restrict, const posix_spawnattr_t *__restrict, char *const [__restrict], char *const [__restrict]);
int posix_spawnp(pid_t *__restrict, const char *__restrict, const posix_spawn_file_actions_t *__restrict, const posix_spawnattr_t *__restrict, char *const [__restrict], char *const [__restrict]);

#ifdef __cplusplus
}
#endif
#endif
