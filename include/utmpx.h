#ifndef __UTMPX_H
#define __UTMPX_H

#define __NEED_pid_t
#define __NEED_time_t
#define __NEED_suseconds_t
#define __NEED_struct_timeval
#include <alltypes.h>
#ifdef __cplusplus
extern "C" {
#endif

struct utmpx {
    char ut_user[16];
    char ut_id[16];
    char ut_line[16];
    pid_t ut_pid;
    short ut_type;
    struct timeval ut_tv;
};

enum {
    EMPTY,
    BOOT_TIME,
    OLD_TIME,
    NEW_TIME,
    USER_PROCESS,
    INIT_PROCESS,
    LOGIN_PROCESS,
    DEAD_PROCESS,
};


void          endutxent(void);
struct utmpx *getutxent(void);
struct utmpx *getutxid(const struct utmpx *);
struct utmpx *getutxline(const struct utmpx *);
struct utmpx *pututxline(const struct utmpx *);
void          setutxent(void);
#ifdef __cplusplus
}
#endif
#endif
