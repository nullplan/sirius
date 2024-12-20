#ifndef __SYS_WAIT_H
#define __SYS_WAIT_H

#include <signal.h>
#include <features.h>
#ifdef __cplusplus
extern "C" {
#endif

/* wait status reverse engineered from Linux source:
 * wait status consists of (starting from LSB):
 *      - a 7-bit "signal" field,
 *      - a 1-bit "coredump" field, and
 *      - an 8-bit exit status field.
 *
 * The following events are signalled:
 *  - if the child continued, the status is set to 0xffff
 *  - if the child exited, the signal field is set to 0, and the exit status field is set to the exit status.
 *  - if the child was killed, the signal field is set to the killing signal number
 *  - if the child was stopped, the signal field is set to 0x7f and the exit status field is set to the stopping signal.
 *
 *  So we get the following table
 *
 *  s\e     |   0   |   1..fe   | ff
 *  0       | exited|   exited  | exited
 *  1..7e   | killed|   invalid | invalid
 *  7f      | killed|   stopped | continued
 *
 *  Remember that on MIPS, signal 127 actually exists.
 */

#define WEXITSTATUS(s)  ((s) >> 8)
#define WTERMSIG(s)     ((s) & 0x7f)
#define WCOREDUMP(s)    ((s) & 0x80)
#define WIFEXITED(s)    (WTERMSIG(s) == 0)
#define WIFSIGNALED(s)  (!WEXITSTATUS(s) && WTERMSIG(s))
#define WIFSTOPPED(s)   (WTERMSIG(s) == 0x7f && WEXITSTATUS(s) - 1u < 0xfeu)
#define WSTOPSIG(s)     WEXITSTATUS(s)

#define WNOHANG         0x00000001
#define WSTOPPED        0x00000002
#define WEXITED         0x00000004
#define WNOWAIT         0x01000000
#define __WNOTHREAD     0x20000000
#define __WALL          0x40000000
#define __WCLONE        0x80000000

#ifdef _XOPEN_SOURCE
#define WCONTINUED      0x00000008
#define WIFCONTINUED(s) ((s) == 0xffff)
#endif

typedef enum {
    P_ALL, P_PID, P_PGID, P_PIDFD
} idtype_t;

pid_t wait(int *);
int waitid(idtype_t , id_t, siginfo_t *, int);
pid_t waitpid(pid_t, int *, int);

#ifdef __cplusplus
}
#endif
#endif
