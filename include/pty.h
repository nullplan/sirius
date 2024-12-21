#ifndef __PTY_H
#define __PTY_H

#include <termios.h>
#ifdef __cplusplus
extern "C" {
#endif

int openpty(int *, int *, char *, const struct termios *, const struct winsize *);
pid_t forkpty(int *, char *, const struct termios *, const struct winsize *);
int login_tty(int);

#ifdef __cplusplus
}
#endif
#endif
